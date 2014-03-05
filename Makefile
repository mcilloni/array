CC=clang
AR=ar
RANLIB=ranlib

all: clean
	$(CC) -c array.c -g -Wall -pedantic -fPIC
	$(AR) -rc libarray.a array.o
	$(RANLIB) libarray.a
	$(CC) -o ex ex.c -L. -larray -ljemalloc -Wall -pedantic -g
	$(CC) -o strex strex.c -L. -larray -ljemalloc -Wall -pedantic -g
	$(CC) -o allocex allocex.c -L. -larray -ljemalloc -Wall -pedantic -g
	$(CC) -o libarray.so array.o -shared 
	rm -f *.o

clean:
	rm -f libarray.{a,so}
	rm -f *.{o,gch}
	rm -f ex strex allocex

