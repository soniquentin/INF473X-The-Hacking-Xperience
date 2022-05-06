#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Missing argument.\n");
        return 1;
    }

    // Get the arguments
    char *ip_adress = argv[1];
    int port = atoi(argv[2]); //atoi convert string to int
    //

    char msg_non_pointer[500];
    char *msg = msg_non_pointer;


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in dest;
    memset(&dest,0,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, ip_adress, &(dest.sin_addr)); // Ip adress adding


    while (1) {
        printf("Enter your message (max length = 500/line): ");
        fgets(msg, 500, stdin);
        int len_msg = strlen(msg);

        if (len_msg == 1) { //case where there is only '/n'
            printf("PROCESS HAS BEEN ENDED UP.\n");
            return 0;
        };

        int sent_size = sendto(sockfd, msg, len_msg, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr));

        printf("Size of data successfully sent: %d\n", sent_size);

    };

    return 0;
};
