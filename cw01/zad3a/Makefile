



static:
	make clean
	gcc -Wall -c main.c
	gcc -Wall -c address_book.c
	ar crs libaddress_book.a address_book.o
	gcc -Wall main.c -o main -L. -laddress_book
	rm -f *.o
	./main >> results.txt

shared:
	gcc -Wall -fPIC -c address_book.c -o address_book.o
	gcc -Wall -shared -o libaddress_book.so address_book.o
	gcc -Wall -Wl,-rpath=. -L. -o main_sh main.c -laddress_book
	./main_sh >> results.txt

dynamic:
	gcc -Wall -fPIC -c address_book.c -o address_book.o
	gcc -Wall -Wl,-rpath=. -fPIC -shared -o libaddress_book.so address_book.o
	gcc -Wall -L. -o main_dynam main.c -ldl -D DLL
	./main_dynam >> results.txt

clean:
	rm -f *.o *.a *.so main
