#ifndef BIPOLAR_FUNCTION_H
#define BIPOLAR_FUNCTION_H

#include "ActivationFunction.h"

class BipolarFunction : public ActivationFunction {
public:
	BipolarFunction(float beta = 1);

protected:
	virtual float calcValue(float arg);
	virtual float calcDerivative(float arg);

private:
	float beta_;
};

#endif
