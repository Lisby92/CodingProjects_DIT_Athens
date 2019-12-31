#ifndef __TRAININGDATA_HPP__
#define __TRAININGDATA_HPP__
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>
using namespace std;

class TrainingData
{
public:
  TrainingData(const string filename);
  bool isEof(void);
  void getTopology(int * newTopology, int & topologySize);
  // Returns the number of input values read from file
  int getNextInputs(double * inputVals);
  int getTargetOutputs(double * targetVals);

private:
  ifstream trainingDataFile;
};


#endif