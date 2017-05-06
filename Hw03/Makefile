CC = g++

all: shell

shell: shell.o lib.o
	$(CC) shell.o lib.o -o shell
shell.o: shell.cpp 
	$(CC) -c shell.cpp -o shell.o
%.o: %.cpp %.h
	$(CC) -c $< -o $@



clean:
	rm shell.o lib.o
	rm shell