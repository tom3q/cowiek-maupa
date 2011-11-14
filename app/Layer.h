#ifndef LAYER_H
#define LAYER_H

class Layer
{
public:
	Layer(int neuronCount, int activationFunction);
	~Layer();

	int neuronCount;
	int activationFunction;
};

#endif