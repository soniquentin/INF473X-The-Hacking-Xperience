/*
 * mutex.c
 *
 *  Created on: Mar 19, 2016
 *      Author: jiaziyi
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 50
void *increase_counter(void *);


int  counter = 0;

int main() {
	//create 50 threads
	pthread_t threads_non_pointer[NTHREADS];
	pthread_t *threads = threads_non_pointer;

	//Threads creation
	for (int i = 0 ; i < NTHREADS; i++) {
        if ( pthread_create( threads + i, NULL, increase_counter, NULL) != 0 ) {
            printf( "ERROR IN THREAD CREATION");
            return 1;
        }
    };

	//Threads joins to ensure they finish before main
	for (int i = 0 ; i < NTHREADS; i++ ) {
		if ( pthread_join( threads[i], NULL ) != 0 ) {
			printf( "PROBLEM WAITING THREAD");
			return 1;
		};
	};



	printf("\nFinal counter value: %d\n", counter);
};

void *increase_counter(void *arg) {

	printf("Thread number %ld, working on counter. The current value is %d\n", (long)pthread_self(), counter);
	int i = counter;
	//usleep (10000); //simulate the data processing
	counter = i+1;

}
