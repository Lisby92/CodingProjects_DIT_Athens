CC = g++
CFLAGS = -g -Wall

NNet: main.o Neuron.o NeuralNetwork.o TrainingData.o
	$(CC) $(CFLAGS) -o NNet main.o Neuron.o NeuralNetwork.o TrainingData.o

Neuron.o: Neuron.cpp
	$(CC) $(CFLAGS) -c Neuron.cpp -o Neuron.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

NeuralNetwork.o: NeuralNetwork.cpp
	$(CC) $(CFLAGS) -c NeuralNetwork.cpp -o NeuralNetwork.o

TrainingData.o: TrainingData.cpp
	$(CC) $(CFLAGS) -c TrainingData.cpp -o TrainingData.o

.PHONY: clean

clean:
	rm -f NNet NNet.o main.o Neuron.o NeuralNetwork.o TrainingData.o