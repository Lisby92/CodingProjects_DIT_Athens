#ifndef __NEURON_HPP__
#define __NEURON_HPP__
#include <cmath>
using namespace std;
class Neuron
{
private:
  // 1.5
  double val;
  // 0 - 1
  double activatedVal;
  // derivative of activated
  double derivedVal;

public:
  // Constructor
  Neuron(double);
  // Destructor
  ~Neuron();
  // Fast Sigmoid Function(Non linear, Easy)
  // f(x) = x / (1 + |x|)
  void activate();
  // Derivative for fast sigmoid function
  // f'(x) = f(x) * (1 - f(x))
  void derive();
  // getters
  double getVal() const;
  double getActivatedVal() const;
  double getDerivedVal() const;
  //setters
  void setVal(const double &);
};

#endif