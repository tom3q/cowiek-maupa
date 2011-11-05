#include <cmath>
#include "BipolarFunction.h"

BipolarFunction::BipolarFunction(double beta)
	: beta_(beta)
{
}

double BipolarFunction::value(double arg) {
	double tmp = exp(2.0*beta_*arg);
	return (double) (tmp-1.0)/(tmp+1.0);
}

double BipolarFunction::derivative(double sum) {
	double tmp = value(sum);
	return (double) beta_*(1.0-tmp*tmp);
}
