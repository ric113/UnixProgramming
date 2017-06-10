CC	= g++
CFLAGS	= -Wall -g -I/usr/include/ncurses
LDFLAGS	= -lncurses -pthread
PROGS	= hw4

all: $(PROGS)

hw4: hw4.o othello.o socket.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o *~ $(PROGS)
