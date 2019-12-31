CC = g++
CFLAGS = -g -Wall -std=c++11 -pthread
all: dropbox_client dropbox_server

dropbox_client: Dropbox_client.o HelpersClient.o
	$(CC) $(CFLAGS) -o dropbox_client Dropbox_client.o HelpersClient.o

dropbox_server: Dropbox_server.o HelpersServer.o
	$(CC) $(CFLAGS) -o dropbox_server Dropbox_server.o HelpersServer.o	

Dropbox_client.o: Dropbox_client.cpp
	$(CC) $(CFLAGS) -c Dropbox_client.cpp -o Dropbox_client.o

Dropbox_server.o: Dropbox_server.cpp
	$(CC) $(CFLAGS) -c Dropbox_server.cpp -o Dropbox_server.o	

HelpersClient.o: HelpersClient.cpp
	$(CC) $(CFLAGS) -c HelpersClient.cpp -o HelpersClient.o

HelpersServer.o: HelpersServer.cpp
	$(CC) $(CFLAGS) -c HelpersServer.cpp -o HelpersServer.o

.PHONY: clean

clean:
	rm -f dropbox_client dropbox_server Dropbox_client.o Dropbox_server.o HelpersServer.o HelpersClient.o