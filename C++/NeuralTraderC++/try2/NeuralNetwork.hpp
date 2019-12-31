#ifndef __NEURALNETWORK_HPP__
#define __NEURALNETWORK_HPP__

#include "Neuron.hpp"

#include <cassert>
using namespace std;

class NeuralNetwork
{
public:

  NeuralNetwork(int * const newTopology, const int newLayers, const int inpSize);
  ~NeuralNetwork();
  void feedForward(double * const inputVals, const int & inpSize);
  void backProp(double * const targetVals);
  void getResults(double * resultVals);//reads output vals and adds in a container
  int getLayerSize(const int & layerIndex) const;//gets size of current layer
  void setInputSize(const int & newInpSize);
  double getRecentAverageError() const;

private:

  Neuron *** layers;
  //Neuron ** biasNeuron; an thes na valeis kai bias parakato
  int * topology;
  int numLayers;
  int inputSize;
  double netError;
  double recentAverageError;
  double recentAverageSmoothingFactor;
  int numResults;
};

#endif