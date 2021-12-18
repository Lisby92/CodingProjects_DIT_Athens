CC = g++
CFLAGS = -g -Wall -std=c++11

project1: main.o helpers.o
	$(CC) $(CFLAGS) -o project1 main.o helpers.o

helpers.o: helpers.cpp
	$(CC) $(CFLAGS) -c helpers.cpp -o helpers.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

.PHONY: clean

clean:
	rm -f project1 project1.o helpers.o main.o