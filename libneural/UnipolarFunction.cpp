#include <cmath>
#include "UnipolarFunction.h"

UnipolarFunction::UnipolarFunction(float beta)
	: beta_(beta) {
}

float UnipolarFunction::calcValue(float arg) {
	float val = (float) 1.0/(1.0+exp(-beta_*arg));
	return val;
}

/*	TODO:
	Calculate derivative by hand and update the formula.
	*/
float UnipolarFunction::calcDerivative(float arg) {
	float tmp = value(arg);
	float val = (float) beta_*tmp*(1.0-tmp);
	return val;
}
