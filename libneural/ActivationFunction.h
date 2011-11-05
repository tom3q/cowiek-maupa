#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

class ActivationFunction {
public:
	virtual double value(double sum) = 0;
	virtual double derivative(double arg) = 0;
};

#endif
