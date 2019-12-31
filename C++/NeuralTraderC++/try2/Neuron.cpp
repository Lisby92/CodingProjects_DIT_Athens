#include "Neuron.hpp"

Neuron::Neuron(const int newIndex, const int newSize)
: outputVal(0.0), index(newIndex), weightSize(newSize), gradient(0.0)
{
  if(weightSize != 0)//output layer
  {
    weight = new double[weightSize];
    for(int i = 0; i < weightSize; i++)
      weight[i] = genRandWeight();
    deltaWeight = new double[weightSize];
    //for(int i = 0; i < weightSize; i++)
      //deltaWeight[i] = genRandWeight();
  }
  else
  {
    weight = NULL;
    deltaWeight = NULL;
  }
  cout << "Neuron[" << index << "] constructed!\n";

  // tune parametres below
  eta = 0.15; // [0.0 - 1.0] overall net training rate, 0.0 slow, 0.2 medium, 1.0 reckless learner
  alpha = 0.5; // [0.0 - n] multiplier of last weight 0.0 no momentum, 0.5 moderate momentum
}

Neuron::~Neuron()
{
  cout << "Neuron[" << index << "] destroyed...\n";
  if(deltaWeight != NULL)
    delete[] deltaWeight;
  if(weight != NULL)
    delete[] weight;
}

double Neuron::genRandWeight() const
{
  double random0To1 = ((double) rand() / (RAND_MAX));
  return random0To1;
}

int Neuron::getWeightSize() const
{
  return weightSize;
}

void Neuron::setOutputVal(const double & newVal)
{
  outputVal = newVal;
}

double Neuron::getOutputVal() const
{
  return outputVal;
}

double Neuron::getOutputWeight(const int & weightIndex) const
{
  return weight[weightIndex];
}

double Neuron::getOutputDeltaWeight(const int & deltaWeightIndex) const
{
  return deltaWeight[deltaWeightIndex];
}

double Neuron::transferFunction(const double & x)
{
  // Fast Sigmoid Function(Non linear, Easy)
  // f(x) = x / (1 + |x|)
  double activatedVal = tanh(x);
  return activatedVal;
}

double Neuron::transferFunctionDerivative(const double & x)
{
  // Derivative for fast sigmoid function
  // f'(x) = f(x) * (1 - f(x))
  double derivedVal = 1.0 - x * x;
  return derivedVal;
}

void Neuron::feedForward(Neuron *** const availLayers, const int & prevLayerSize, const int & layerIndex)
{
  double sum = 0.0;

  // Sum the previous layer's outputs (which are our inputs)
  // Include the bias neuron from the previous layer
  for(int j = 0; j < prevLayerSize; j++)//this will include bias if added
  {
    sum += (availLayers[layerIndex][j]->getOutputVal() * 
            availLayers[layerIndex][j]->getOutputWeight(index));
  }
  double newOutputVal = transferFunction(sum);
  setOutputVal(newOutputVal);
}

void Neuron::calcOutputGradients(const double & targetVal)
{
  double delta = targetVal - outputVal;
  gradient = delta * transferFunctionDerivative(outputVal);
  cout << "gradient is in output " << gradient << endl;
}

void Neuron::calcHiddenGradients(Neuron *** const availLayers, const int & nextLayerIndex, const int & nextLayerSize)
{
  double dow = sumDOW(availLayers, nextLayerIndex, nextLayerSize);
  gradient = dow * transferFunctionDerivative(outputVal);
  cout << "gradient is in hidden " << gradient << endl;
}

void Neuron::updateInputWeights(Neuron *** availLayers, const int & prevLayerIndex, const int & prevLayerSize)
{
  // The weights to be updated are at the weight array of the previous layer neurons
  int prevNeuron;
  double oldDeltaWeight;
  double newDeltaWeight;
  double oldWeight = 0.0;
  double newWeight = 0.0;
  for(int j = 0; j < prevLayerSize; j++)
  {
    prevNeuron = j;
    oldDeltaWeight = availLayers[prevLayerIndex][prevNeuron]->getOutputDeltaWeight(index); 
    newDeltaWeight = 
    // Individual input, magnified by the gradient and train rate:
    eta
    * availLayers[prevLayerIndex][prevNeuron]->getOutputVal()
    * gradient
    // Also add momentum == a fraction of the previous delta weight
    + alpha 
    * oldDeltaWeight;

    availLayers[prevLayerIndex][prevNeuron]->setDeltaWeight(index, newDeltaWeight);
    oldWeight = availLayers[prevLayerIndex][prevNeuron]->getOutputWeight(index);
    newWeight = oldWeight + newDeltaWeight;
    availLayers[prevLayerIndex][prevNeuron]->setWeight(index, newWeight);
  }

}

double Neuron::sumDOW(Neuron *** const availLayers, const int &nextLayerIndex, const int & nextLayerSize)
{
  // Sum our contributions of the errors at the neurons we feed

  double sum = 0.0;
  for(int j = 0; j < nextLayerSize -1; j++) // needs -1 to exclude bias
  {
    sum += availLayers[nextLayerIndex][j]->getOutputWeight(j) *
    availLayers[nextLayerIndex][j]->gradient;
  }
  return sum;
}

void Neuron::setDeltaWeight(const int & indexN, const double & newDeltaWeight)
{
  deltaWeight[indexN] = newDeltaWeight;
}

void Neuron::setWeight(const int & indexN, const double & newWeight)
{
  weight[indexN] = newWeight;
}