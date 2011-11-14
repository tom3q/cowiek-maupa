#include "Layer.h"

Layer::Layer(int neuronCount, int activationFunction)
{
	this->neuronCount = neuronCount;
	this->activationFunction = activationFunction;
}


Layer::~Layer()
{
}
