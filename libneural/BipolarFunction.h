#ifndef BIPOLAR_FUNCTION_H
#define BIPOLAR_FUNCTION_H

#include "ActivationFunction.h"

class BipolarFunction : public ActivationFunction {
public:
	BipolarFunction(double beta = 1);

	virtual double value(double arg);
	virtual double derivative(double arg);

private:
	double beta_;
};

#endif
