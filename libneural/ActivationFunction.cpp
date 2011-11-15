#include "ActivationFunction.h"
#include <limits>

ActivationFunction::ActivationFunction()
{
	valarg_ = std::numeric_limits<double>::infinity();
	drvarg_ = std::numeric_limits<double>::infinity();
}

float ActivationFunction::value(float arg)
{
	if (arg == valarg_)
		return val_;
	valarg_ = arg;
	val_ = calcValue(arg);
	return val_;
}

float ActivationFunction::derivative(float arg)
{
	if (arg == drvarg_)
		return drv_;
	float tmp = value(arg);
	drvarg_ = arg;
	drv_ = calcDerivative(arg);
	return drv_;
}
