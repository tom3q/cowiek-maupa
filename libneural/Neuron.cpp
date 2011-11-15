#include "Neuron.h"
#include "UnipolarFunction.h"

Neuron::Neuron() {
	func_ = boost::shared_ptr<ActivationFunction>(new UnipolarFunction());
	value_ = drv_ = 0;
}

Neuron::Neuron(boost::shared_ptr<ActivationFunction> f) {
	func_ = f;
	value_ = drv_ = 0;
}

void Neuron::setSum(double sum) {
	value_ = func_.get()->value(sum);
	drv_ = func_.get()->derivative(sum);
}
