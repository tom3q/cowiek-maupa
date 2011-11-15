#ifndef UNIPOLAR_FUNCTION_H
#define UNIPOLAR_FUNCTION_H

#include "ActivationFunction.h"

class UnipolarFunction : public ActivationFunction {
public:
	UnipolarFunction(float beta = 1);

protected:
	virtual float calcValue(float arg);
	virtual float calcDerivative(float arg);

private:
	float beta_;
};

#endif
