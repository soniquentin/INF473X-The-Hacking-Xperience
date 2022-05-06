#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define NMAX_CLIENTS 50

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int available_threads = NMAX_CLIENTS; //Count the threads that are available to process a request


// Use a struct to pass many information in pthread_create
struct _process_request_args_ {
    int _sockfd_;
    struct sockaddr_in _from_;
    int _index_threads_;
};


void *process_request(void *data) {
    int sockfd = ( (struct _process_request_args_*) data)->_sockfd_ ;
    struct sockaddr_in from = ( (struct _process_request_args_*) data)->_from_ ;
    int index_threads = ( (struct _process_request_args_*) data)->_index_threads_ ;

    memset(&from,0,sizeof(from));
    socklen_t fromlen = sizeof(struct sockaddr);

    char buf_non_pointer[500];
    char *buf = buf_non_pointer; //buf stocks info received

    while (1) {
        //Listen to message and store in buf
        int received_size = recvfrom(sockfd, buf, 500, 0, (struct sockaddr*) &from, &fromlen);
        available_threads--;
        pthread_mutex_lock(&myMutex);
        printf("[START] %d is treating a request ",pthread_self());
        printf("(received size : %d) \n",received_size);
        pthread_mutex_unlock(&myMutex);
        //printf("Size of data successfully received (max length : 500): %d\n", received_size);
        //buf[received_size] = '\0';

        //usleep(10000000); //Simulate a process that takes time
        //Resent to "from" the message stored in buf


        int resent_size = sendto(sockfd, buf, received_size, 0, (struct sockaddr*) &from, sizeof(struct sockaddr));
        available_threads++;
        printf("[END] %d has finished treating its request \n",index_threads);

    };
};



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


    //Prepare structure _send_request_args_
    struct sockaddr_in from_list[NMAX_CLIENTS];

    //Threads creation
    pthread_t threads_non_pointer[NMAX_CLIENTS];
    pthread_t *threads = threads_non_pointer;
	for (int i = 0 ; i < NMAX_CLIENTS; i++) {

        //Creating structure _send_request_args_ to store the info
        struct _process_request_args_ args ;
        args._sockfd_ = sockfd;
        args._from_ = from_list[i];
        args._index_threads_ = i;
        if ( pthread_create( threads + i, NULL, process_request, &args) != 0 ) {
            printf( "ERROR IN THREAD CREATION");
            return 1;
        }
    };


    //Main's thread used to tell clients if the server is too busy
    while (1) {
        if (available_threads == 0) {
            pthread_mutex_lock(&myMutex);
            struct sockaddr_in from; // set "from" (client)
            memset(&from,0,sizeof(from));
            socklen_t fromlen = sizeof(struct sockaddr);
            char buf_non_pointer[500];
            char *buf = buf_non_pointer; //buf stock info received

            //Listen to message and store in buf
            int received_size = recvfrom(sockfd, buf, 500, 0, (struct sockaddr*) &from, &fromlen);
            char busy_nonpointer[] = "Cannot process your request (No thread available)\n";
            char *busy = busy_nonpointer;
            int resent_size = sendto(sockfd, busy, strlen(busy), 0, (struct sockaddr*) &from, sizeof(struct sockaddr));
            pthread_mutex_unlock(&myMutex);
        };
    };

    return 0;
};
