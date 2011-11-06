#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "TrainingSet.h"
#include "NeuralNetwork.h"

class Supervisor
{
public:
	Supervisor();
	virtual ~Supervisor();

	void setTrainingSet(TrainingSet& data);
	void setNeuralNetwork(NeuralNetwork& newNet);

	virtual void train() = 0;
	double totalError();

protected:
	virtual void buildData() = 0;

	TrainingSet* trainingSet_;
	NeuralNetwork* net_;
};

#endif
