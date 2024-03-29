#include <vector>
#include "Supervisor.h"

Supervisor::Supervisor() 
	: trainingSet_(0), net_(0)
{
}

Supervisor::~Supervisor() 
{
}

void Supervisor::setTrainingSet(TrainingSet& data) {
	trainingSet_ = &data;
}

void Supervisor::setNeuralNetwork(NeuralNetwork& newNet) {
	if (!newNet.isCurrentSupervisor(this)) {
		newNet.setCurrentSupervisor(this);
		net_ = &newNet;
		buildData();
	}
}

double Supervisor::totalError() {
	std::vector<double> output;
	double error = 0, d;

	for (int i=0; i<trainingSet_->size(); ++i) {
		std::vector<double>& desOutput = trainingSet_->getDesiredOutput(i);
		output = net_->getOutput(trainingSet_->getInput(i));
		for (unsigned int j=0; j<output.size(); j++) {
			d = desOutput[j] - output[j];
			error += d*d;
		}
	}
	return error;
}
