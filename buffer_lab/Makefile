.PHONY: clean all client server

all: client.o server.o

client.o: client.c
	gcc -std=c99 -o client.o client.c

server.o: server.c
	gcc -std=c99 -o server.o server.c

client:
	./client.o 127.0.0.1

server:
	./server.o

clean:
	rm *.o