#include "SupervisorThread.h"

SupervisorThread::SupervisorThread(QObject *parent) : QThread(parent)
{
	stopped = true;
	i = 0;
}

SupervisorThread::~SupervisorThread()
{
}

void SupervisorThread::run()
{
	do {
		// network logic here
		++i;
		emit setEpoch(i);
		usleep(500);
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
	supervisor.setNeuralNetwork(net);
}

void SupervisorThread::setTrainingSet(TrainingSet &set)
{
	supervisor.setTrainingSet(set);
}