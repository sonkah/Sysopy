CC = gcc
flag = -pedantic -std=c11 -Wall -D_POSIX_SOURCE -D_XOPEN_SOURCE

all: lib_queue.a barber.c client.c
	$(CC) $(flag) -pthread barber.c -o barber -g lib_queue.a -lrt
	$(CC) $(flag) -pthread client.c -o client -g lib_queue.a -lrt

lib_queue.a: queue.o
	ar rc $@ $^
	ranlib $@

queue.o: queue.c queue.h
	$(CC) $(flag) -c $< -o $@

clean:
	rm -f *.o
