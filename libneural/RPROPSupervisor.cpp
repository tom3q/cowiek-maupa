#include "RPROPSupervisor.h"

RPROPSupervisor::RPROPSupervisor()
	: trainingSet_(0), net_(0), nMax_(50), nMin_(0.0002), a_(1.2), b_(0.5)
{
}

RPROPSupervisor::RPROPSupervisor(double min, double max)
	: trainingSet_(0), net_(0), nMax_(max), nMin_(min)
{
}

RPROPSupervisor::~RPROPSupervisor() {
	if (trainingSet_ != 0)
		delete trainingSet_;
}

void RPROPSupervisor::setTrainingSet(TrainingSet& data) {
	trainingSet_ = new TrainingSet(data);
}

void RPROPSupervisor::setNeuralNetwork(NeuralNetwork& net) {
	net_ = &net;
	buildData();
}

void RPROPSupervisor::buildData() {
	// get rid of previous data
	n_.clear();
	errorDrv_.clear();
	prevDrv_.clear();
	delta_.clear();
	
	// create matrices holding n(t) and derivative information
	for (int i=0; i<net_->layerCount(); ++i) {
		Matrix2D tmpMatrix(net_->connectionMatrix(i).getWidth(), 
						net_->connectionMatrix(i).getHeight());

		// n(t)
		tmpMatrix.fill(0.5);
		n_.push_back(tmpMatrix);

		// derivative matrices
		errorDrv_.push_back(tmpMatrix);
		prevDrv_.push_back(tmpMatrix);

		// add delta vector
		std::vector<double> v;
		delta_.push_back(v);
	}
}

double sgn(double a) {
	return a > 0 ? 1 : -1;
}

void RPROPSupervisor::train() {
	std::vector<double> output;
	int layerCount = net_->layerCount();
	double delta, derivative, eps;

	// clear error derivative matrices
	for (int l=0; l<layerCount; l++)
		errorDrv_[l].fill(0);

	for (int i=0; i<trainingSet_->size(); ++i) {
		// calculate network output
		std::vector<double>& desOutput = trainingSet_->getDesiredOutput(i);
		output = net_->getOutput(trainingSet_->getInput(i));

		for (int l=layerCount-1; l>=0; l--) {
			delta_[l].clear();
			for (unsigned int j=0; j<net_->neurons(l).size(); ++j) {
				// get neuron derivative
				derivative = net_->neurons(l)[j].getDerivative();

				// get delta. depends on if it is the last layer or not
				if (l < layerCount-1) {
					Matrix2D& connMatrix = net_->connectionMatrix(l+1);
					eps = 0;
					for (unsigned int k=0; k<net_->neurons(l+1).size(); k++)
						eps += delta_[l+1][k] * connMatrix.at(k, j+1);
					delta = eps * derivative;
				}
				else
					delta = (desOutput[j] - output[j])*derivative;

				// store delta for further processing
				delta_[l].push_back(delta);

				// store delta in treshold connection
				errorDrv_[l].at(j, 0) += delta;

				// update error derivative matrix
				if (l == 0) {
					std::vector<double>& prev = trainingSet_->getInput(i);
					for (unsigned int k=0; k<prev.size(); ++k) 
						errorDrv_[l].at(j, k+1) += delta * prev[k];
				} else {
					std::vector<Neuron>& prev = net_->neurons(l-1);
					for (unsigned int k=0; k<prev.size(); ++k) 
						errorDrv_[l].at(j, k+1) += delta * prev[k].getValue();
				}
			}
		}
	}

	// update n(t) matrices using information stored in error derivative matrices
	for (int l=0; l<layerCount; ++l) {
		Matrix2D& nMatrix = n_[l];
		Matrix2D& prevErrMatrix = prevDrv_[l];
		Matrix2D& errorMatrix = errorDrv_[l];
		Matrix2D& connMatrix = net_->connectionMatrix(l);

		for (int x=0; x<nMatrix.getWidth(); ++x)
		for (int y=0; y<nMatrix.getHeight(); ++y) {
			if (errorMatrix.at(x, y) * prevErrMatrix.at(x, y) > 0) 
				nMatrix.at(x, y) = a_*nMatrix.at(x, y) < nMax_ ? a_*nMatrix.at(x, y) : nMax_;
			else if (errorMatrix.at(x, y) * prevErrMatrix.at(x, y) < 0)
				nMatrix.at(x, y) = b_*nMatrix.at(x, y) > nMin_ ? b_*nMatrix.at(x, y) : nMin_;

			prevErrMatrix.at(x, y) = errorMatrix.at(x, y);
			connMatrix.at(x, y) += nMatrix.at(x, y) * sgn(errorMatrix.at(x, y));
		}
	}
}

double RPROPSupervisor::totalError() {
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
