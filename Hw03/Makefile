CC = g++

all: shell

shell: shell.o lib.o unixpipe.o
	$(CC) shell.o lib.o unixpipe.o -o shell
shell.o: shell.cpp 
	$(CC) -c shell.cpp -o shell.o
%.o: %.cpp %.h
	$(CC) -c $< -o $@



clean:
	rm shell.o lib.o unixpipe.o
	rm shell