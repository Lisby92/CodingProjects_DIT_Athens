CC = g++
CFLAGS = -g -Wall

project1: Building.o A1_0.o
	$(CC) $(CFLAGS) -o project1 Building.o A1_0.o

Building.o: Building.cpp
	$(CC) $(CFLAGS) -c Building.cpp -o Building.o

A1_0.o: A1_0.cpp
	$(CC) $(CFLAGS) -c A1_0.cpp -o A1_0.o

.PHONY: clean

clean:
	rm -f project1 project1.o Building.o A1_0.o
