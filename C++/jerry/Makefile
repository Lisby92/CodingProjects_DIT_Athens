CC = g++
CFLAGS = -g -Wall

ergasia1: List.o Main.o
	$(CC) $(CFLAGS) -o ergasia1 List.o Main.o

List.o: List.cpp
	$(CC) $(CFLAGS) -c List.cpp -o List.o

Main.o: Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp -o Main.o

.PHONY: clean

clean:
	rm -f ergasia1 ergasia1.o List.o Main.o
