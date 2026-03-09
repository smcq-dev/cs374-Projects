CC=gcc
CCOPTS=-Wall -Wextra -Werror
LIBS=-lpthread

.PHONY: all clean

all: pc

pc: pc.c eventbuf.c
	$(CC) $(CCOPTS) -o pc pc.c eventbuf.c $(LIBS)

clean:
	rm -f pc