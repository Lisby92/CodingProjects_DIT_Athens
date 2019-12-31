#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int * newTopology, const int newLayers, const int inpSize)
: topology(newTopology), numLayers(newLayers), inputSize(0), netError(0.0), recentAverageError(0.0), recentAverageSmoothingFactor(100.0), numResults(newTopology[numLayers-1])//paikse me to recentsmootihing 
{
  layers = new Neuron **[newLayers];//layers = topology size
  int flag = 0;
  for(int i = 0; i < newLayers; i++)
  {
    layers[i] = new Neuron*[topology[i]];//each layer consists of topology[i] neurons
    if(i == (newLayers - 1))//last layer as output layer has no further outputs
      flag = 1;
    for(int j = 0; j < topology[i]; j++)
    {
      if(flag == 0)//input layer or hidden
        layers[i][j] = new Neuron(j, topology[i+1]);
      else//output layer
        layers[i][j] = new Neuron(j, 0);
    }
    if(i == 0)
      cout << "Input ";
    else if(i == (newLayers - 1))
      cout << "Output ";
    else
      cout << "Hidden ";
    cout << "Layer[" << i << "] constructed!\n";
  }
  cout << "A new NeuralNetwork has been constructed!\n";
}

NeuralNetwork::~NeuralNetwork()
{
  for(int i = 0; i < numLayers; i++)
  {
    for(int j = 0; j < getLayerSize(i); j++)
      delete layers[i][j];
    delete[] layers[i];
    if(i == 0)
      cout << "Input ";
    else if(i == (numLayers - 1))
      cout << "Output ";
    else
      cout << "Hidden ";
    cout << "Layer[" << i << "] destroyed!\n";
  }
  delete[] layers;
  delete[] topology;
  cout << "NeuralNetwork destroyed!\n";
}

void NeuralNetwork::feedForward(double * const inputVals, const int & inpSize)
{
  setInputSize(inpSize);
  assert(inputSize == getLayerSize(0));

  //Assign (latch) the input values into the input neurons
  for(int i = 0; i < inputSize; i++)
    layers[0][i]->setOutputVal(inputVals[i]);

  //Forward propagate
  for(int i = 1; i < numLayers; i++)//input is set start from first hidden layer
  {
    for(int j = 0; j < getLayerSize(i); j++)//needs -1 for bias adding
      layers[i][j]->feedForward(layers, getLayerSize(i-1), i-1);//the j-th neuron and the previous layer
  }
}

void NeuralNetwork::backProp(double * const targetVals)
{
  // Calculate overall net error (RMS of output neuron errors)
  double delta;
  int outputLayerSize = getLayerSize(numLayers-1);
  int hiddenLayerSize;
  int nextLayerSize;
  int prevLayerSize;
  int curLayerSize;
  netError = 0.0;
  for(int j = 0; j < outputLayerSize; j++)//gia na valeis kai to bias thelei -1
  {
    delta = targetVals[j] - layers[numLayers-1][j]->getOutputVal();
    netError += (delta * delta);
  }
  netError /= outputLayerSize - 1; // get average error squared
  netError = sqrt(netError);   // RMS

  // Implement a recent average measuremnet:

  recentAverageError = (recentAverageError * recentAverageSmoothingFactor + netError) / (recentAverageSmoothingFactor + 1.0);

  // Calculate output layer gradients

  for(int j = 0; j < outputLayerSize; j++)//gia na valeis kai to bias thelei -1
    layers[numLayers-1][j]->calcOutputGradients(targetVals[j]);

  // Calculate hidden layer gradients

  for(int i = (numLayers - 2); i > 0; i--)//skip output layer and go backwards
  {
    hiddenLayerSize = getLayerSize(i);
    nextLayerSize = getLayerSize(i+1);
    for(int j = 0; j < hiddenLayerSize; j++)
      layers[i][j]->calcHiddenGradients(layers, (i+1), nextLayerSize);//pass index to next layer
  }

  // For all layers from outputs to first hidden layer update weights

  for(int i = (numLayers - 1); i > 0; i--)
  {
    curLayerSize = getLayerSize(i);
    prevLayerSize = getLayerSize(i-1);
    for(int j = 0; j < curLayerSize; j++)//-1 an thes bias
      layers[i][j]->updateInputWeights(layers, (i-1), prevLayerSize);// pass index to previous layer
  }
}

void NeuralNetwork::getResults(double * resultVals)
{
  for(int i = 0; i < numResults; i++)
    resultVals[i] = -1.0;//initialise
  for(int j = 0; j < numResults; j++)
    resultVals[j] = layers[numLayers-1][j]->getOutputVal();
}

void NeuralNetwork::setInputSize(const int & newInpSize)
{
  inputSize = newInpSize;
}

int NeuralNetwork::getLayerSize(const int & layerIndex) const
{
  return topology[layerIndex];
}

double NeuralNetwork::getRecentAverageError() const
{
  return recentAverageError;
}