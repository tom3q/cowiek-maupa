#ifndef UNIPOLAR_FUNCTION_H
#define UNIPOLAR_FUNCTION_H

#include "ActivationFunction.h"

class UnipolarFunction : public ActivationFunction {
public:
	UnipolarFunction(double beta = 1);

	virtual double value(double arg);
	virtual double derivative(double arg);

private:
	double beta_;
};

#endif
