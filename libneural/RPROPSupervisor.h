#ifndef RPROP_SUPERVISOR_H
#define RPROP_SUPERVISOR_H

#include <vector>
#include "TrainingSet.h"
#include "NeuralNetwork.h"
#include "Matrix2D.h"

class RPROPSupervisor
{
public:
	RPROPSupervisor();
	RPROPSupervisor(double min, double max);
	virtual ~RPROPSupervisor();

	void setTrainingSet(TrainingSet& data);
	void setNeuralNetwork(NeuralNetwork& net);
	void buildData();
	void train();

	double totalError();

private:
	TrainingSet* trainingSet_;
	NeuralNetwork* net_;
	std::vector<Matrix2D> n_, errorDrv_, prevDrv_;
	std::vector<std::vector<double> > delta_;
	double nMax_, nMin_, a_, b_;
};

#endif
