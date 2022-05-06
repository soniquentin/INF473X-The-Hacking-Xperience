#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Missing argument.\n");
        return 1;
    }

    // Get the arguments
    int port = atoi(argv[1]); //atoi convert string to int
    //


    // Set the server
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (const struct sockaddr *) &server_addr, sizeof(struct sockaddr));


    struct sockaddr_in from; // set "from" (client)
    memset(&from,0,sizeof(from));
    socklen_t fromlen = sizeof(struct sockaddr);
    char buf_non_pointer[500];
    char *buf = buf_non_pointer; //*buf stock info received

    while (strstr(buf, "exit") == NULL) {
        //Listen to message and store in buf
        int received_size = recvfrom(sockfd, buf, 500, 0, (struct sockaddr*) &from, &fromlen);
        printf("Size of data successfully received (max length : 500): %d\n", received_size);
        //buf[received_size] = '\0';


        //Resent to "from" the message stored in buf
        int resent_size = sendto(sockfd, buf, received_size, 0, (struct sockaddr*) &from, sizeof(struct sockaddr));
        printf("Resent size: %d\n", resent_size);
    };


    return 0;
};
