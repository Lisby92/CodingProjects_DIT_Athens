#ifndef __LAYER_HPP__
#define __LAYER_HPP__

#include <iostream>
#include "Neuron.hpp"
#include "Matrix.hpp"

class Layer
{
private:
  int size;
  Neuron** neurons;
  
public:
  // Constructor
  Layer(int);
  // Destructor
  ~Layer();
  void setVal(const int &, const double &);
  Matrix * matrixifyVals();
  Matrix * matrixifyActivatedVals();
  Matrix * matrixifyDerivedVals();
  Neuron ** getNeurons() const;
  void setNeurons(Neuron **);
};

#endif