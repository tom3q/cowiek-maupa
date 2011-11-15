#include <windows.h>
#include <boost\random.hpp>
#include "NeuralNetwork.h"
#include "Supervisor.h"
#include "LinearFunction.h"
#include "UnipolarFunction.h"
#include "BipolarFunction.h"
#include <stdexcept>

typedef std::vector<Neuron> Neurons;

NeuralNetwork::NeuralNetwork() 
	: inputCount_(0), prevInputCount_(0), supervisor_(0) {
}

NeuralNetwork::~NeuralNetwork() {
}

void NeuralNetwork::addInputLayer(int inputs) {
	inputCount_ = inputs;
	prevInputCount_ = inputCount_;
}

void NeuralNetwork::addLayer(int nCount, int actFunction) {
	// if no input was specified, throw an exception
	if (inputCount_ == 0) {
		throw std::invalid_argument("no input specified");
		return;
	}

	// create a proper activation function
	boost::shared_ptr<ActivationFunction> funcPtr;
	switch (actFunction) {
		case FUNCTION_UNIPOLAR: 
			funcPtr = boost::shared_ptr<ActivationFunction>(new UnipolarFunction());
			break;
		case FUNCTION_BIPOLAR: 
			funcPtr = boost::shared_ptr<ActivationFunction>(new BipolarFunction());
			break;
		case FUNCTION_LINEAR:
			funcPtr = boost::shared_ptr<ActivationFunction>(new LinearFunction());
	}

	// create a layer of neurons
	Neurons neurons;
	for (int i=0; i<nCount; i++)
		neurons.push_back(Neuron(funcPtr));

	// create a matrix and fill it with 1
	Matrix2D matrix(nCount, prevInputCount_ + 1);
	matrix.fill(1);

	// memorize that last added layer had nCount neurons
	prevInputCount_ = nCount;

	// put a new layer into layer vector
	layers_.push_back(boost::make_tuple(matrix, neurons));
}

std::vector<double> NeuralNetwork::getOutput(std::vector<double> &input) {
	// create result
	std::vector<double> result;
	
	// create input matrix
	Matrix2D vect(input.size() + 1, 1);
	vect.at(0, 0) = 1.0;
	for (unsigned int i=0; i<input.size(); ++i)
		vect.at(i+1, 0) = input[i];

	for (unsigned int i=0; i<layers_.size(); ++i) {
		Matrix2D sums(vect * layers_[i].get<0>());

		Neurons &neurons = layers_[i].get<1>();
		for (unsigned int j=0; j<neurons.size(); ++j)
			neurons[j].setSum(sums.at(j, 0));

		if (i < layers_.size()-1) {
			vect = Matrix2D(neurons.size() + 1, 1);
			vect.at(0, 0) = 1.0;
			for (unsigned int j=0; j<neurons.size(); ++j)
				vect.at(j+1, 0) = neurons[j].getValue();
		}
		else {
			for (unsigned int j=0; j<neurons.size(); ++j)
				result.push_back(neurons[j].getValue());
		}
	}

	return result;
}

Matrix2D& NeuralNetwork::connectionMatrix(int layer) {
	return layers_[layer].get<0>();
}

Neurons& NeuralNetwork::neurons(int layer) {
	return layers_[layer].get<1>();
}

int NeuralNetwork::layerCount() const {
	return layers_.size();
}

int NeuralNetwork::outputNeuronCount() const {
	return layers_[layers_.size()-1].get<1>().size();
}

bool NeuralNetwork::isCurrentSupervisor(Supervisor *supervisor) {
	return supervisor_ == supervisor;
}

void NeuralNetwork::setCurrentSupervisor(Supervisor *supervisor) {
	supervisor_ = supervisor;
}

void NeuralNetwork::randomizeConnections(double range) {
	SYSTEMTIME st;
	GetSystemTime (&st);

	boost::random::mt19937 rng(st.wSecond * 100 + st.wMilliseconds);
	boost::random::uniform_real_distribution<> random(-range, range);

	// randomization
	for (unsigned int i=0; i<layers_.size(); i++) {
		Matrix2D& conn = layers_[i].get<0>();
		for (int x=0; x<conn.getWidth(); x++)
		for (int y=0; y<conn.getHeight(); y++)
			conn.at(x, y) = (double) random(rng);
	}

	// set current supervisor pointer to 0, so that the next time 
	// a supervisor runs train() it rebuilds its data.
	supervisor_ = 0;
}
