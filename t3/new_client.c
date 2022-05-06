#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

// Use a struct to pass many information in pthread_create
struct _send_request_args_ {
    int _sockfd_;
    char *_msg_;
    struct sockaddr_in _dest_;
};


void *send_request(void *data) {
    //These variables only defined in send_request
    int sockfd = ( (struct _send_request_args_ *) data)->_sockfd_;
    char *msg = ( (struct _send_request_args_*) data)->_msg_ ;
    struct sockaddr_in dest = ( (struct _send_request_args_ *) data)->_dest_ ;

    while (1) {
        printf("Enter your message (max length = 500/line): ");
        fgets(msg, 500, stdin);
        int len_msg = strlen(msg);

        int sent_size = sendto(sockfd, msg, len_msg, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr));
        //printf("Size of data successfully sent: %d\n", sent_size);
    };
};



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

    //Set the destination (server) idendity
    struct sockaddr_in dest;
    memset(&dest,0,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, ip_adress, &(dest.sin_addr)); // Ip adress adding


    //Creating structure _send_request_args_ to store the info
    struct _send_request_args_ args ;
    args._sockfd_ = sockfd;
    args._msg_ = msg;
    args._dest_ = dest;


    struct sockaddr_in from; // set "from" (server response)
    memset(&from,0,sizeof(from));
    socklen_t fromlen = sizeof(struct sockaddr);
    char buf_non_pointer[500];
    char *buf = buf_non_pointer; //*buf stock info received


    pthread_t request_thread;
    if (pthread_create(&request_thread, NULL, send_request, &args) != 0) {
        fprintf(stderr, "Fail request_thread creation\n");
        return 1;
    };


    // The reiceive's thread is the main's one
    while (1) {
        int received_size = recvfrom(sockfd, buf, 500, 0, (struct sockaddr*) &dest, &fromlen);
        printf("Response from the server: %s\n", buf);
    };

    return 0;
};
