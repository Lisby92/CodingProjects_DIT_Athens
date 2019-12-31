CC=gcc
CFLAGS=-Wall

Trapeza: TStoixeiouOuras.o ch6_Simulation.o ch6_QArray.o Tameio.o
	$(CC) $(CFLAGS) -o Trapeza TStoixeiouOuras.o ch6_Simulation.o ch6_QArray.o Tameio.o

TStoixeiouOuras.o: TStoixeiouOuras.c
	$(CC) $(CFLAGS) -c TStoixeiouOuras.c -o TStoixeiouOuras.o

ch6_Simulation.o: ch6_Simulation.c
	$(CC) $(CFLAGS) -c ch6_Simulation.c -o ch6_Simulation.o

ch6_QArray.o: ch6_QArray.c
	$(CC) $(CFLAGS) -c ch6_QArray.c -o ch6_QArray.o

Tameio.o: Tameio.c
	$(CC) $(CFLAGS) -c Tameio.c -o Tameio.o

.PHONY: clean

clean:
	rm -f test test.o
