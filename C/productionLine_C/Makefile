CC = gcc
CFLAGS = -g -Wall

productionLine: productionLine.o
	$(CC) $(CFLAGS) -o productionLine productionLine.o

productionLine.o: productionLine.c
	$(CC) $(CFLAGS) -c productionLine.c -o productionLine.o

.PHONY: clean

clean:
	rm -f productionLine productionLine.o