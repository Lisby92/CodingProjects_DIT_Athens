#include <iostream>

#include "Matrix.hpp"

Matrix::Matrix(int rows, int cols, bool isRandom)
: numRows(rows), numCols(cols)
{
  double r;
  values = new double*[numRows];
  for(int i = 0; i < numRows; i++)
      values[i] = new double[numCols];

  for(int i = 0; i < numRows; i++)
  {
    for(int j = 0; j < numCols; j++)
    {
      r = 0.00;
      if(isRandom == true)
        r = getRandomNumber();
      values[i][j] = r;
    }
  }
  //cout << "Matrix constructed successfully!\n";
}

Matrix::~Matrix()
{
  for(int i = 0; i < numRows; i++)
    delete[] values[i];
  delete[] values;
  //cout << "Matrix destructed successfully!\n";
}

void Matrix::setVal(const int &row, const int &col , const double &val)
{
  //cout << values[row][col] << " was the old val \n";
  values[row][col] = val;
  //cout << val << " is the new val\n";
}

double Matrix::getVal(const int & row, const int & col) const
{
  return values[row][col];
}

Matrix * Matrix::transpose()
{
  Matrix * transposedMatrix = new Matrix(numCols, numRows, false);
  for(int i = 0; i < numRows; i++)
    for(int j = 0; j < numCols; j++)
      transposedMatrix->setVal(j, i, this->getVal(i, j));
  return transposedMatrix;
}

double Matrix::getRandomNumber() const
{
  double random0To1 = ((double) rand() / (RAND_MAX));
  return random0To1;
}

int Matrix::getNumRows() const
{
  return numRows;
}

int Matrix::getNumCols() const
{
  return numCols;
}

void Matrix::printConsole() const
{
  for(int i = 0; i < numRows; i++)
  {
    //cout << "row " << i << endl;
    for(int j = 0; j < numCols; j++)
    {
      //cout << "col " << j << endl;
      cout << "|" << values[i][j] << "| "; //<< endl;
    }
    cout << "\n\n";
  }
}
