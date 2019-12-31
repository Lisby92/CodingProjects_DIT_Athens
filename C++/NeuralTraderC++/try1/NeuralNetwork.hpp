#ifndef __NEURAL_NETWORK_HPP__
#define __NEURAL_NETWORK_HPP__

#include <iostream>

#include "Layer.hpp"

class NeuralNetwork
{
private:
  int * topology;
  int topologySize;
  Layer ** layers;
  Matrix ** weightMatrices;
  double * input;
  int inputSize;

public: 
  NeuralNetwork(int *, int);
  ~NeuralNetwork();
  Matrix * getNeuronMatrix(const int &);
  Matrix * getActivatedNeuronMatrix(const int &);
  Matrix * getDerivedNeuronMatrix(const int &);
  Matrix * getWeightMatrix(const int &);
  void setCurInput(double *, const int &);
  void setNeuronVal(const int &, const int &, const double &);
  void printConsole() const;
  void feedForward();
  Matrix * multiplyMatrices(Matrix *, Matrix *);// this is a seprate utility
};

#endif