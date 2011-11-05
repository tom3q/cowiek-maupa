#include <cmath>
#include "UnipolarFunction.h"

UnipolarFunction::UnipolarFunction(double beta)
	: beta_(beta) {
}

double UnipolarFunction::value(double arg) {
	return (double) 1.0/(1.0+exp(-beta_*arg));
}

/*	TODO:
	Calculate derivative by hand and update the formula.
	*/
double UnipolarFunction::derivative(double arg) {
	double tmp = value(arg);
	return (double) beta_*tmp*(1.0-tmp);
}
