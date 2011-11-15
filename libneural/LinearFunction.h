#ifndef LINEAR_FUNCTION_H
#define LINEAR_FUNCTION_H

#include "ActivationFunction.h"

class LinearFunction : public ActivationFunction
{
protected:
	virtual float calcValue(float sum);
	virtual float calcDerivative(float arg);
};

#endif
