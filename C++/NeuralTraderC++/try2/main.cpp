#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include "TrainingData.hpp"

#include "NeuralNetwork.hpp"

using namespace std;

void showArrayVals(string outLabel, double * const array, const int & arraySize)
{
  cout << outLabel << " ";
  for(int i = 0; i < arraySize; i++)
    cout << array[i] << " ";
  cout << endl;
}

int main(int argc, char ** argv)
{
  TrainingData trainData("trainingData.txt");
  time_t t;
  srand(time(&t));
  double * inputVals;
  double * targetVals;
  double * resultVals;
  int * topology = new int[3];//isos prepei na allaxthei se vector pantou gia to xrisimo .size ...less cancer kai pio dynamiko, isos
  int topologySize;//to vazo tria tora gt exo topologia 2 4 1 ("trainingData.txt") ara ksero apo prin
  trainData.getTopology(topology, topologySize);
  NeuralNetwork myNNet(topology, topologySize, topology[0]);//topology, numLayers, inpSize
  int trainingPass = 0;
  int inputsSize = topology[0];//first layer = input
  int resultsSize = topology[topologySize-1];//last layer = output
  int targetsSize = topology[topologySize-1];
  inputVals = new double[inputsSize];
  targetVals = new double[targetsSize];
  resultVals = new double[resultsSize];
  while(!trainData.isEof())
  {
    trainingPass++;
    cout << endl << "Pass: " << trainingPass;

    // Get new input data and feed it forward :
    if(trainData.getNextInputs(inputVals) != topology[0])
      break;
    showArrayVals("Inputs:", inputVals, inputsSize);
    myNNet.feedForward(inputVals, topology[0]);

    // Collect the Neural Network's actual results:
    myNNet.getResults(resultVals);
    showArrayVals("Outputs:", resultVals, resultsSize);

    // Train the Neural Network what the outputs should have been
    targetsSize = trainData.getTargetOutputs(targetVals);
    showArrayVals("Targets:", targetVals, targetsSize);
    assert(targetsSize == topology[topologySize-1]);
    myNNet.backProp(targetVals);

    // Report how well the training is working, averaged over recent samples:
    cout << "Net recent average error: "
         << myNNet.getRecentAverageError() << endl;
  }
  cout << endl << "DONE!" << endl;
  delete[] inputVals;
  delete[] targetVals;
  delete[] resultVals;

  return 0;
}