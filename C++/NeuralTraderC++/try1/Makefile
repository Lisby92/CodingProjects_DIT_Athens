CC = g++
CFLAGS = -g -Wall

NNTrader: NeuralTrader.o Layer.o Neuron.o Matrix.o NeuralNetwork.o
	$(CC) $(CFLAGS) -o NNTrader NeuralTrader.o Layer.o Neuron.o Matrix.o NeuralNetwork.o

Neuron.o: Neuron.cpp
	$(CC) $(CFLAGS) -c Neuron.cpp -o Neuron.o

Layer.o: Layer.cpp
	$(CC) $(CFLAGS) -c Layer.cpp -o Layer.o

Matrix.o: Matrix.cpp
	$(CC) $(CFLAGS) -c Matrix.cpp -o Matrix.o

NeuralTrader.o: NeuralTrader.cpp
	$(CC) $(CFLAGS) -c NeuralTrader.cpp -o NeuralTrader.o

NeuralNetwork.o: NeuralNetwork.cpp
	$(CC) $(CFLAGS) -c NeuralNetwork.cpp -o NeuralNetwork.o

.PHONY: clean

clean:
	rm -f NNTrader NNTrader.o NeuralTrader.o Layer.o Neuron.o Matrix.o NeuralNetwork.o