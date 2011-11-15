#include "SupervisorThread.h"

SupervisorThread::SupervisorThread(QObject *parent) : QThread(parent)
{
	stopped = true;
	setTerminationEnabled(true);
	init();
}

SupervisorThread::~SupervisorThread()
{
}

void SupervisorThread::init()
{
	epochs = 0;
	lastError = 0;
	dead = 0;
	randomized = 0;
}

void SupervisorThread::run()
{
	const double errorDelta = 0.001, deadCount = 10;
	double error;

	do {
		error = supervisor.totalError();
		if (error < 0.001)
			break;

		supervisor.train();

		if (abs(error - lastError) < errorDelta) {
			dead++;
			if (dead == deadCount) {
				net_->randomizeConnections(5.0);
				dead = epochs = 0;
				randomized++;
			}
		}
		else {
			dead = 0;
			lastError = error;
		}

		++epochs;
		emit setEpoch(epochs);
	}
	while(!stopped);
}

void SupervisorThread::stop()
{
	stopped = true;
}

void SupervisorThread::setStopped(bool stopped)
{
	this->stopped = stopped;
}

void SupervisorThread::setNeuralNetwork(NeuralNetwork &net)
{
	net_ = &net;
	supervisor.setNeuralNetwork(net);
}

void SupervisorThread::setTrainingSet(TrainingSet &set)
{
	supervisor.setTrainingSet(set);
}