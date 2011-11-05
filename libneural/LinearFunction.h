#ifndef LINEAR_FUNCTION_H
#define LINEAR_FUNCTION_H

#include "ActivationFunction.h"

class LinearFunction : public ActivationFunction
{
public:
	virtual double value(double sum);
	virtual double derivative(double arg);
};

#endif
