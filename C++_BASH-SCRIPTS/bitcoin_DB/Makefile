CC = g++
CFLAGS = -g -Wall -std=c++11

bitcoin: Driver.o HashTable.o TransactionsList.o TimeFrame.o BlockchainTree.o Wallet.o Helpers.o
	$(CC) $(CFLAGS) -o bitcoin Driver.o HashTable.o TransactionsList.o TimeFrame.o BlockchainTree.o Wallet.o Helpers.o

HashTable.o: HashTable.cpp
	$(CC) $(CFLAGS) -c HashTable.cpp -o HashTable.o

Driver.o: Driver.cpp
	$(CC) $(CFLAGS) -c Driver.cpp -o Driver.o

TransactionsList.o: TransactionsList.cpp
	$(CC) $(CFLAGS) -c TransactionsList.cpp -o TransactionsList.o

TimeFrame.o: TimeFrame.cpp
	$(CC) $(CFLAGS) -c TimeFrame.cpp -o TimeFrame.o

BlockchainTree.o: BlockchainTree.cpp
	$(CC) $(CFLAGS) -c BlockchainTree.cpp -o BlockchainTree.o	

Wallet.o: Wallet.cpp
	$(CC) $(CFLAGS) -c Wallet.cpp -o Wallet.o	

Helpers.o: Helpers.cpp
	$(CC) $(CFLAGS) -c Helpers.cpp -o Helpers.o

.PHONY: clean

clean:
	rm -f bitcoin bitcoin.o Driver.o HashTable.o TransactionsList.o TimeFrame.o BlockchainTree.o Wallet.o Helpers.o