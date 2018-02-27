
static:
	make clean
	gcc -Wall -c $(CFLAGS) main.c
	gcc -Wall -c address_book.c
	ar crs libaddress_book.a address_book.o
	gcc -Wall main.c -o main -L. -laddress_book
	rm -f *.o
	./main >> results.txt

shared:
	gcc -Wall -fPIC -c $(CFLAGS) address_book.c -o address_book.o
	gcc -Wall -shared -o libaddress_book.so address_book.o
	gcc -Wall -Wl,-rpath=. -L. -o main_sh main.c -laddress_book
	./main_sh >> results.txt

dynamic:
	gcc -Wall -fPIC -c $(CFLAGS) address_book.c -o address_book.o
	gcc -Wall -Wl,-rpath=. -fPIC -shared -o libaddress_book.so address_book.o
	gcc -Wall -L. -o main_dynam main.c -ldl -D DLL
	./main_dynam >> results.txt
tests:
	make static CFLAGS=-O0
	make static CFLAGS=-O1
	make static CFLAGS=-O2
	make static CFLAGS=-O3
	make static CFLAGS=-Os
	make dynamic CFLAGS=-O0
	make dynamic CFLAGS=-O1
	make dynamic CFLAGS=-O2
	make dynamic CFLAGS=-O3
	make dynamic CFLAGS=-Os
	make shared CFLAGS=-O0
	make shared CFLAGS=-O1
	make shared CFLAGS=-O2
	make shared CFLAGS=-O3
	make shared CFLAGS=-Os
clean:
	rm -f *.o *.a *.so main
