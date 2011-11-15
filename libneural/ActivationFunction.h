#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

class ActivationFunction {
protected:
	ActivationFunction();
	virtual float calcValue(float sum) { return 0; };
	virtual float calcDerivative(float arg) { return 0; };
public:
	float value(float arg);
	float derivative(float arg);

private:
	float valarg_;
	float val_;
	float drvarg_;
	float drv_;
};

#endif
