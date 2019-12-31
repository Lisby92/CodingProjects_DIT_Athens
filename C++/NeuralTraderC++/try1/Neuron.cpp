#include "Neuron.hpp"

#include <iostream>

using namespace std;

Neuron::Neuron(double newVal)
{
  //cout << "A new neuron has been constructed!\n";
  val = newVal;
  activate();
  derive();
}

Neuron::~Neuron()
{
  //cout << "Neuron destroyed...\n";
}

void Neuron::activate()
{
  activatedVal = val / (1 + abs(val));
}

void Neuron::derive()
{
  derivedVal = activatedVal * (1 - activatedVal);
}

double Neuron::getVal() const
{
  return val;
}

double Neuron::getActivatedVal() const
{
  return activatedVal;
}

double Neuron::getDerivedVal() const
{
  return derivedVal;
}

void Neuron::setVal(const double & newVal)
{
  val = newVal;
}