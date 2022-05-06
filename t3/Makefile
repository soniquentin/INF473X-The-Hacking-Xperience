all: mutex new_server new_client

mutex: mutex.o
	cc -g -o mutex mutex.o -pthread

new_server: new_server.o
	cc -g -o new_server new_server.o -pthread

new_client: new_client.o
	cc -g -o new_client new_client.o -pthread

clean:
	rm -f mutex mutex.o
	rm -f new_server new_server.o
	rm -f new_client new_client.o

%.o: %.c
	cc -g3 -Wall -o $@ -c $<
