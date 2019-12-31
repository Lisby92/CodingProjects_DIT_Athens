CC = g++
CFLAGS = -g -Wall

projectEco: EcoSim.o Tile.o Date.o Map.o Plant.o Animal.o List.o
	$(CC) $(CFLAGS) -o projectEco EcoSim.o Tile.o Date.o Map.o Plant.o Animal.o List.o

EcoSim.o: EcoSim.cpp
	$(CC) $(CFLAGS) -c EcoSim.cpp -o EcoSim.o

Tile.o: Tile.cpp
	$(CC) $(CFLAGS) -c Tile.cpp -o Tile.o

Date.o: Date.cpp
	$(CC) $(CFLAGS) -c Date.cpp -o Date.o

Map.o: Map.cpp
	$(CC) $(CFLAGS) -c Map.cpp -o Map.o	

Plant.o: Plant.cpp
	$(CC) $(CFLAGS) -c Plant.cpp -o Plant.o	

Animal.o: Animal.cpp
	$(CC) $(CFLAGS) -c Animal.cpp -o Animal.o			

List.o: List.cpp
	$(CC) $(CFLAGS) -c List.cpp -o List.o

.PHONY: clean

clean:
	rm -f projectEco projectEco.o EcoSim.o Tile.o Date.o Map.o Plant.o Animal.o List.o