#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int * newTopology, int newTopSize)
: topology(newTopology), topologySize(newTopSize)
{
  //cout << "size of topology " << topologySize << endl;
  // create the layers 1st = input, middle = hidden, last = output
  layers = new Layer*[topologySize];
  for(int i = 0; i < topologySize; i++)
    layers[i] = new Layer(topology[i]);//each layer has topology[i] neurons
  // create the weighted matrices
  weightMatrices = new Matrix*[topologySize-1];//they must be 1 less than layers
  for(int i = 0; i < (topologySize - 1); i++)
  {
    weightMatrices[i] = new Matrix(topology[i], topology[i+1], true);
    //weightMatrices[i]->printConsole();
  }
  //cout << "Neural network constructed successfully!\n";
}

NeuralNetwork::~NeuralNetwork()
{
  //delete topology;
  for(int i = 0; i < topologySize; i++)
    delete layers[i];
  delete[] layers;
  for(int i = 0; i < (topologySize - 1); i++)
    delete weightMatrices[i];
  delete[] weightMatrices;
  //cout << "Neural network destroyed successfully!\n";
}

void NeuralNetwork::setCurInput(double * newInput, const int & newSize)
{
  input = newInput;
  inputSize = newSize;
  for(int i = 0; i < newSize; i++)//init input layer
    layers[0]->setVal(i, input[i]);
}

void NeuralNetwork::setNeuronVal(const int & layerIndex, const int & neuronIndex, const double & newVal)
{
  layers[layerIndex]->setVal(neuronIndex, newVal);
}

void NeuralNetwork::printConsole() const
{
  for(int i = 0; i < topologySize; i++)
  {
    cout << "LAYER: " << i;
    if(i == 0)
    {
      cout << " INPUT\n";
      Matrix * m = layers[i]->matrixifyVals();
      m->printConsole();
      delete m;
    }
    else
    {
      cout << " HIDDEN\n";
      Matrix * m = layers[i]->matrixifyActivatedVals();
      m->printConsole();
      delete m;
    }
  }
}

Matrix * NeuralNetwork::getNeuronMatrix(const int & index)
{
  return layers[index]->matrixifyVals();
}

Matrix * NeuralNetwork::getActivatedNeuronMatrix(const int & index)
{
  return layers[index]->matrixifyActivatedVals();
}

Matrix * NeuralNetwork::getDerivedNeuronMatrix(const int & index)
{
  return layers[index]->matrixifyDerivedVals();
}

Matrix * NeuralNetwork::getWeightMatrix(const int & index)
{
  return weightMatrices[index];
}

void NeuralNetwork::feedForward()
{
  for(int i = 0; i < (topologySize - 1); i++)
  {
    Matrix * a;
    if(i == 0)//input
      a = getNeuronMatrix(i);
    if(i != 0)//hidden
      a = getActivatedNeuronMatrix(i);
    Matrix * b = getWeightMatrix(i);
    Matrix * c = multiplyMatrices(a, b);// c = a * b dot product
    int colsC = c->getNumCols();
    for(int indexC = 0; indexC < colsC; indexC++)
      setNeuronVal((i + 1), indexC, c->getVal(0, indexC));
    delete a;//whereas here its needed since there is a new operation
    //delete b;not needed here just returning pointer 
    delete c;//here needed also
  }
  
}
// this is a seprate utility
Matrix * NeuralNetwork::multiplyMatrices(Matrix * a, Matrix * b)
{
  int rowsA = a->getNumRows();
  int colsB = b->getNumCols();
  int rowsB = b->getNumRows();
  double product = 0.0;
  double newVal = 0.0;
  Matrix * c = new Matrix(rowsA, colsB, false);
  for(int i = 0; i < rowsA; i++)
  {
    for(int j = 0; j < colsB; j++)
    {
      for(int k = 0; k < rowsB; k++)
      {
        product = (a->getVal(i, k) * b->getVal(k, j));
        newVal = (c->getVal(i, j) + product);
        c->setVal(i, j, newVal);
      }
    }
  }
  return c;
}