/*
 * rawip_example.c
 *
 *  Created on: May 4, 2016
 *      Author: jiaziyi
 */


#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include "header.h"

#define SRC_IP  "192.168.1.111" //set your source ip here. It can be a fake one
#define SRC_PORT 54321 //set the source port here. It can be a fake one

#define DEST_IP "129.104.89.108" //set your destination ip here
#define DEST_PORT 5555 //set the destination port here
#define TEST_STRING "test data" //a test string as packet payload


int main(int argc, char *argv[])
{
	char source_ip[] = SRC_IP;
	char dest_ip[] = DEST_IP;


	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

	int on = 1;
	setsockopt(fd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}

	char packet[65536], *data, *pseudogram;
	char data_string[] = TEST_STRING;
	memset(packet, 0, 65536);

	//IP header pointer
	struct iphdr *iph = (struct iphdr *)packet;

	//UDP header pointer
	struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct iphdr));
	struct pseudo_udp_header psh; //pseudo header

	//data section pointer
	data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

	//fill the data section
	strncpy(data, data_string, strlen(data_string));

	//fill the IP header here
	//(inspired by https://www.binarytides.com/raw-udp-sockets-c-linux/)
	iph->version = 4; //Version of IP (IP4)
	iph->ihl = 5; //Internet Header Length (IHL) --> minimum value 5
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
	iph->id = htonl(0); //Id of this packet
	iph->frag_off = 0; //Fragment offset
	iph->ttl = 255; //Time to live (TTL) --> datagram's lifetime
	iph->protocol = 17; //17 for UDP //https://en.wikipedia.org/wiki/List_of_IP_protocol_numbers//
	iph->check = 0; //Checksum set to 0 at the beginning
	iph->saddr = inet_addr(source_ip);
	inet_pton(AF_INET, dest_ip, &(iph->daddr));
	iph->check = checksum((unsigned short *) packet, sizeof(struct iphdr));
	//print_ip_header(packet, iph->tot_len);


	//fill the UDP header
	udph->source = htons(SRC_PORT);
	udph->dest = htons(DEST_PORT);
	udph->len = htons(8 + strlen(data));
	udph->check = 0;

	//fill the pseudo header
	inet_pton(AF_INET, source_ip, &(psh.source_address));
	inet_pton(AF_INET, dest_ip, &(psh.dest_address));
	psh.placeholder = 0;
	psh.protocol = 17;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));

	//udp's checksum
	int psize = sizeof(struct pseudo_udp_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = malloc(psize);
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_udp_header));
	memcpy(pseudogram + sizeof(struct pseudo_udp_header) , udph , sizeof(struct udphdr) + strlen(data));
	iph->check = checksum((unsigned short *) pseudogram, psize);

	//send the packet
	struct sockaddr_in dest;
    memset(&dest,0,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(DEST_PORT);
	inet_pton(AF_INET, dest_ip, &(dest.sin_addr));

	int sent_len = sendto(fd, packet, iph->tot_len, 0, (struct sockaddr *) &dest, sizeof(struct sockaddr));
	printf("Packet sent !");

	return 0;

}
