CC = gcc
CFLAGS = -g -Wall

Words: ch8_AVLpointer.o ch8_BSTpointerR.o typos_stoixeiouDDA.o Words.o main.o
	$(CC) $(CFLAGS) -o Words ch8_AVLpointer.o ch8_BSTpointerR.o typos_stoixeiouDDA.o Words.o main.o

typos_stoixeiouDDA.o: typos_stoixeiouDDA.c
	$(CC) $(CFLAGS) -c typos_stoixeiouDDA.c -o typos_stoixeiouDDA.o

ch8_AVLpointer.o: ch8_AVLpointer.c
	$(CC) $(CFLAGS) -c ch8_AVLpointer.c -o ch8_AVLpointer.o

ch8_BSTpointerR.o: ch8_BSTpointerR.c
	$(CC) $(CFLAGS) -c ch8_BSTpointerR.c -o ch8_BSTpointerR.o

Words.o: Words.c
	$(CC) $(CFLAGS) -c Words.c -o Words.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

.PHONY: clean

clean:
	rm -f Words ch8_AVLpointer.o ch8_BSTpointerR.o typos_stoixeiouDDA.o Words.o main.o