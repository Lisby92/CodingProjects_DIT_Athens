CC = g++
CFLAGS = -g -Wall -std=c++11

mirror_client: MirrorClient.o Helpers.o
	$(CC) $(CFLAGS) -o mirror_client MirrorClient.o Helpers.o

MirrorClient.o: MirrorClient.cpp
	$(CC) $(CFLAGS) -c MirrorClient.cpp -o MirrorClient.o

Helpers.o: Helpers.cpp
	$(CC) $(CFLAGS) -c Helpers.cpp -o Helpers.o

.PHONY: clean

clean:
	rm -f mirror_client mirror_client.o MirrorClient.o Helpers.o