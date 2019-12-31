CC = gcc
CFLAGS = -g -Wall

MENU: TyposStoixeiouMenu.o Menu.o main.o Configurations.o
	$(CC) $(CFLAGS) -o MENU TyposStoixeiouMenu.o Menu.o main.o Configurations.o

Configurations.o: Configurations.c
	$(CC) $(CFLAGS) -c Configurations.c -o Configurations.o

TyposStoixeiouMenu.o: TyposStoixeiouMenu.c
	$(CC) $(CFLAGS) -c TyposStoixeiouMenu.c -o TyposStoixeiouMenu.o

Menu.o: Menu.c
	$(CC) $(CFLAGS) -c Menu.c -o Menu.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

.PHONY: clean

clean:
	rm -f MENU TyposStoixeiouMenu.o Menu.o main.o Configurations.o