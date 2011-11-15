#ifndef NEURON_H
#define NEURON_H

#include <boost\shared_ptr.hpp>
#include "ActivationFunction.h"

class Neuron {
public: 
	Neuron();
	Neuron(boost::shared_ptr<ActivationFunction> f);

	inline double getValue() const {
		return value_;
	}

	inline double getDerivative() const {
		return drv_;
	}

	void setSum(double sum);

protected:
	boost::shared_ptr<ActivationFunction> func_;
	double value_, drv_;
};

#endif
