#ifndef SIGMOID_FUNCTION_H
#define SIGMOID_FUNCTION_H

#include "ActivationFunction.h"

class SigmoidFunction :
	public ActivationFunction
{
public:
	virtual NeuronValue value(NeuronValue sum);
	virtual NeuronValue derivative(NeuronValue arg);

private:
	float beta_;
};

#endif
