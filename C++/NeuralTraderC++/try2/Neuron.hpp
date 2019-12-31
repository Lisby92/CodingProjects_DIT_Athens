#ifndef __NEURON_HPP__
#define __NEURON_HPP__

#include <cmath>
#include <iostream>
#include <ctime>

using namespace std;

class Neuron
{
public:

  Neuron(const int newIndex, const int newSize);
  ~Neuron();
  double genRandWeight() const;
  int getWeightSize() const;
  void setOutputVal(const double & newVal);
  double getOutputVal() const;
  double getOutputWeight(const int & weightIndex) const;
  double getOutputDeltaWeight(const int & deltaWeightIndex) const;
  double setDeltaWeight(const int & deltaWeightIndex);
  double transferFunction(const double & x);
  double transferFunctionDerivative(const double & x);
  void feedForward(Neuron *** const availLayers, const int & prevLayerSize, const int & layerIndex);
  void calcOutputGradients(const double & targetVal);
  void calcHiddenGradients(Neuron *** const availLayers, const int & nextLayerIndex, const int & nextLayerSize);
  void updateInputWeights(Neuron *** availLayers, const int & prevLayerIndex, const int & prevLayerSize);
  double sumDOW(Neuron *** const availLayers, const int &nextLayerIndex, const int & nextLayerSize);
  void setDeltaWeight(const int & index, const double & newDeltaWeight);
  void setWeight(const int & indexN, const double & newWeight);

private:
  double outputVal;
  int index;
  double * weight;
  double * deltaWeight;
  int weightSize;//next layer size
  double gradient;
  // below can be static and is tunable
  double eta; // [0.0 - 1.0] overall net training rate
  double alpha; // [0.0 - n] multiplier of last weight change (momentum)

};

#endif