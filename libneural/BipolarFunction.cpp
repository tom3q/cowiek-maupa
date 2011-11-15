#include <cmath>
#include "BipolarFunction.h"

BipolarFunction::BipolarFunction(float beta)
	: beta_(beta)
{
}

float BipolarFunction::calcValue(float arg) {
	float tmp = exp(2.0*beta_*arg);
	float val = (float) (tmp-1.0)/(tmp+1.0);
	return val;
}

float BipolarFunction::calcDerivative(float sum) {
	float tmp = value(sum);
	float val = (float) beta_*(1.0-tmp*tmp);
	return val;
}
