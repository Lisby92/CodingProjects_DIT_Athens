CC = g++
CFLAGS = -g -Wall -std=c++11

project2: Frame.o HashPageTable.o PageReplacement.o MemoryManagerUnit.o
	$(CC) $(CFLAGS) -o project2 Frame.o HashPageTable.o PageReplacement.o MemoryManagerUnit.o

HashPageTable.o: HashPageTable.cpp
	$(CC) $(CFLAGS) -c HashPageTable.cpp -o HashPageTable.o

Frame.o: Frame.cpp
	$(CC) $(CFLAGS) -c Frame.cpp -o Frame.o

PageReplacement.o: PageReplacement.cpp
	$(CC) $(CFLAGS) -c PageReplacement.cpp -o PageReplacement.o

MemoryManagerUnit.o: MemoryManagerUnit.cpp
	$(CC) $(CFLAGS) -c MemoryManagerUnit.cpp -o MemoryManagerUnit.o		

.PHONY: clean

clean:
	rm -f project2 project2.o Frame.o HashPageTable.o PageReplacement.o MemoryManagerUnit.o