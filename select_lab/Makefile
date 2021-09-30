.PHONY: clean all client server

all: client.o server.o

client.o:
	mkdir -p output
	gcc -std=c99 -g -o output/client.o client.c

server.o:
	mkdir -p output
	gcc -std=c99 -g -o output/server.o server.c

client:
	./output/client.o 127.0.0.1

server:
	./output/server.o

clean:
	rm ./output/*.o