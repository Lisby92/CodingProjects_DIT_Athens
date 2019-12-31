#include <iostream>
#include "Layer.hpp"
using namespace std;

Layer::Layer(int newSize)
: size(newSize)
{
  neurons = new Neuron*[size];
  for(int i = 0; i < size; i++)
    neurons[i] = new Neuron(0.00);
  //cout << "Layer constructed successfully!\n";
}

Layer::~Layer()
{
  for(int i = 0; i < size; i++)
    delete neurons[i];
  delete[] neurons;
  //cout << "Layer destroyed successfully!\n";
}

void Layer::setVal(const int & index, const double & value)
{
  //cout << "index " << index << " value " << value << endl;
  neurons[index]->setVal(value);
}

Matrix * Layer::matrixifyVals()
{
  Matrix * m = new Matrix(1, size, false);
  for(int i = 0; i < size; i++)
  {
    m->setVal(0, i, neurons[i]->getVal());
    //cout << "in matrixVals reading i: " << i << " val : " << neurons[i]->getVal() << endl;
  }
  return m;
}

Matrix * Layer::matrixifyActivatedVals()
{
  Matrix * m = new Matrix(1, size, false);
  for(int i = 0; i < size; i++)
    m->setVal(0, i, neurons[i]->getActivatedVal());
  return m;
}

Matrix * Layer::matrixifyDerivedVals()
{
  Matrix * m = new Matrix(1, size, false);
  for(int i = 0; i < size; i++)
    m->setVal(0, i, neurons[i]->getDerivedVal());
  return m;
}

Neuron ** Layer::getNeurons() const
{
  return neurons;
}

void Layer::setNeurons(Neuron ** newNeuronsArray)
{
  neurons = newNeuronsArray;
}