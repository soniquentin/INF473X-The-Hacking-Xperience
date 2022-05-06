/*
 * idserver.c
 *
 *  Created on: Feb 15, 2016
 *      Author: jiaziyi
 *  Updated: JACF, 2020
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "idserver.h"

/**
 * print the server's information
 */
void print_idserver(idserver s){
		printf("Id: %s\n", s.id);
		printf("Latency (usec): %d \n", s.latency);
		printf("Region: %s\n", s.region);
		printf("Status: %s\n", s.status);
		printf("Nbr of threads: %d\n", *(s.nthreads) );



}

/**
 * try to modify the server information
 */
void modify(idserver s, char *_id, int _latency, char _status[]) {
	s.id = _id;
	s.latency = _latency;
	strcpy(s.status, _status);
}

/**
 * try to modify the student information using pointer
 */
void modify_by_pointer(idserver *s, char *_id, int _latency, char _status[])
{	s->id = _id;
	s->latency = _latency;
	strcpy(s->status, _status);
}

idserver* create_idserver(char *_id, char *_region, int _latency,
		char *_status, int *_nthreads)
{

	idserver *s;
	s = (idserver *)malloc(sizeof(idserver));
	s->id = _id;
	s->region = _region;
	s->latency = _latency;
	strncpy(s->status, _status, strlen(_status)+1);
	s->nthreads = _nthreads;
	puts("---print inside create_idserver function---");
	print_idserver(*s);
	puts("---end of print inside");
	return s;
}
