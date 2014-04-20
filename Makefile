ifndef CC
	CC=clang
endif

ifndef AR
	AR=ar 
endif

ifndef RANLIB
	RANLIB=ranlib
endif

all: clean
	$(CC) -c array.c -std=c11 -g -Wall -pedantic -fPIC
	$(AR) rc libarray.a array.o
	$(RANLIB) libarray.a
	$(CC) -o ex ex.c -std=c11 -L. -larray  -Wall -pedantic -g
	$(CC) -o strex strex.c -std=c11 -L. -larray  -Wall -pedantic -g
	$(CC) -o allocex allocex.c -std=c11 -L. -larray  -Wall -pedantic -g
	$(CC) -o slicex slicex.c -std=c11 -L. -larray  -Wall -pedantic -g
	rm -f *.o

clean:
	rm -f libarray.{a,so}
	rm -f *.{o,gch}
	rm -f ex strex allocex slicex

