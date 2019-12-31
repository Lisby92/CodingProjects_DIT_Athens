#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <ctime>
#include "Neuron.hpp"

class Matrix
{
private:
  int numRows;
  int numCols;
  double ** values;
  
public:
  // Constructor
  Matrix(int, int, bool);
  // Destructor
  ~Matrix();
  Matrix * transpose();
  void setVal(const int &, const int &, const double &);
  double getVal(const int &, const int &) const;
  double getRandomNumber() const;
  int getNumRows() const;
  int getNumCols() const;
  void printConsole() const;
};

#endif