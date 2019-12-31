#include "TrainingData.hpp"

using namespace std;

TrainingData::TrainingData(const string filename)
{
  trainingDataFile.open(filename.c_str());
}

int TrainingData::getNextInputs(double * inputVals)
{
  string line;
  getline(trainingDataFile, line);
  stringstream ss(line);

  string label;
  ss >> label;
  int counter = 0;
  if(label.compare("in:") == 0)
  {
    double oneValue;
    while(ss >> oneValue)
      inputVals[counter++] = oneValue;
  }
  return counter;
}

int TrainingData::getTargetOutputs(double * targetVals)
{
  string line;
  getline(trainingDataFile, line);
  stringstream ss(line);

  string label;
  ss >> label;
  int counter = 0;
  if(label.compare("out:") == 0)
  {
    double oneValue;
    while(ss >> oneValue)
      targetVals[counter++] = oneValue;
  }
  return counter;
}

bool TrainingData::isEof(void)
{
  return trainingDataFile.eof();
}

void TrainingData::getTopology(int * newTopology, int & topologySize)
{
  string line;
  string label;
  getline(trainingDataFile, line);
  stringstream ss(line);
  ss >> label;
  if(this->isEof() || (label.compare("topology:") != 0))
    abort();
  int counter = 0;
  while(!ss.eof())
  {
    int n;
    ss >> n;
    newTopology[counter] = n;
    counter++;
  }
  topologySize = counter;
}