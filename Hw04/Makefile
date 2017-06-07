CC	= g++
CFLAGS	= -Wall -g -I/usr/include/ncurses
LDFLAGS	= -lncurses -pthread
PROGS	= sample

all: $(PROGS)

sample: sample.o othello.o socket.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o *~ $(PROGS)
