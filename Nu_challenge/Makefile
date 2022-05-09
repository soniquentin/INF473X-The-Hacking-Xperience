all: overflow_buffer_attack


overflow_buffer_attack.o: overflow_buffer_attack.c
	cc -c -Wall -g overflow_buffer_attack.c 

overflow_buffer_attack: overflow_buffer_attack.o
	cc -g -o overflow_buffer_attack overflow_buffer_attack.o


clean:
	rm overflow_buffer_attack.o overflow_buffer_attack
