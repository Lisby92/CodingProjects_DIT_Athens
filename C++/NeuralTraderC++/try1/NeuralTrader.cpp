#include <iostream>
#include "NeuralNetwork.hpp"

using namespace std;

int main(int argc, char ** argv)
{
  time_t t;
  srand(time(&t));
  //Neural network test
  int * topology = new int[3];
  topology[0] = 3;// input layer with 3 neurons
  topology[1] = 2;// hidden layer with 2 neurons
  topology[2] = 1;// output layer with 1 neuron

  double * input = new double[3];
  input[0] = 1;
  input[1] = 0;
  input[2] = 1;

  NeuralNetwork * nn = new NeuralNetwork(topology, 3);
  nn->setCurInput(input, 3);
   nn->printConsole();
  nn->feedForward();
 

  delete[] topology;
  delete[] input;
  delete nn;

  //Neuron test
  /*
  Neuron * n = new Neuron(0.9);
  cout << "Value: " << n->getVal() << endl;
  cout << "Activated Value: " << n->getActivatedVal() << endl;
  cout << "Derived Value: " << n->getDerivedVal() << endl;
  delete n;
  */
  /*
  //Matrix test
  Matrix * m = new Matrix(3, 2, true);
  m->printConsole();
  cout << "===================\n";
  Matrix * mT = m->transpose();
  mT->printConsole();
  delete m;
  delete mT;
  */
  /*
  //Layer test
  Layer * l = new Layer(10);
  delete l;
  */

  return 0;
}