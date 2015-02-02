ifndef CC
	CC=clang
endif

ifndef AR
	AR=ar 
endif

ifndef RANLIB
	RANLIB=ranlib
endif

ARCH = $(shell uname -m)
UNAME = $(shell uname)

FLAGS =

ifeq ($(ARCH),x86_64)
ifeq (,$(findstring CYGWIN,$(UNAME)))
	FLAGS += -fPIC
endif
endif

.PHONY: all,clean

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(OBJS)
	$(AR) rc libarray.a array.o
	$(RANLIB) libarray.a
	$(CC) -o ex ex.o -std=c99 -L. -larray -g
	$(CC) -o strex strex.o -std=c99 -L. -larray -g
	$(CC) -o allocex allocex.o -std=c99 -L. -larray -g
	$(CC) -o slicex slicex.o -std=c99 -L. -larray -g
	rm -f *.o

%.o: %.c
	$(CC) -c $< -o $@ -std=c99 -g -Wall -pedantic $(FLAGS)

clean:
	rm -f libarray.a
	rm -f $(OBJS)
	rm -f ex strex allocex slicex

