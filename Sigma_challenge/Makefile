all: syn_flooding_multi syn_flooding


syn_flooding_multi: syn_flooding_multi.o
	cc -g -o syn_flooding_multi syn_flooding_multi.o -pthread

syn_flooding: syn_flooding.o
	cc -g -o syn_flooding syn_flooding.o

clean:
	rm -f syn_flooding_multi syn_flooding_multi.o
	rm -f syn_flooding syn_flooding.o

%.o: %.c
	cc -g3 -Wall -o $@ -c $<
