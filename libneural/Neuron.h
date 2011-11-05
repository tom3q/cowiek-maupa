#ifndef NEURON_H
#define NEURON_H

#include <boost\shared_ptr.hpp>
#include "UnipolarFunction.h"

class Neuron {
public: 
	Neuron();
	Neuron(boost::shared_ptr<ActivationFunction> f);

	double getValue();
	double getDerivative();

	void setSum(double sum);

protected:
	boost::shared_ptr<ActivationFunction> func_;
	double value_, drv_;
};

#endif
