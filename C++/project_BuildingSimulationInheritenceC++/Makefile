CC = g++
CFLAGS = -g -Wall

project2: Building2.o B1_0.o
	$(CC) $(CFLAGS) -o project2 Building2.o B1_0.o

Building2.o: Building2.cpp
	$(CC) $(CFLAGS) -c Building2.cpp -o Building2.o

B1_0.o: B1_0.cpp
	$(CC) $(CFLAGS) -c B1_0.cpp -o B1_0.o

.PHONY: clean

clean:
	rm -f project2 project2.o Building2.o B1_0.o