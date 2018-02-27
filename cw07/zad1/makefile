CC = gcc
flag = -pedantic -std=c11 -Wall -D_POSIX_SOURCE -D_XOPEN_SOURCE

all: lib_queue.a barber.c client.c
	$(CC) $(flag) barber.c -o barber -g lib_queue.a
	$(CC) $(flag) client.c -o client -g lib_queue.a

lib_queue.a: queue.o
	ar rc $@ $^
	ranlib $@

queue.o: queue.c queue.h
	$(CC) $(flag) -c $< -o $@

clean:
	rm -f *.o
