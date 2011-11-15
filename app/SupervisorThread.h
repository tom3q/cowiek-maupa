#ifndef SUPERVISORTHREAD_H
#define SUPERVISORTHREAD_H

#include <QThread>
#include "libneural\RPROPSupervisor.h"
#include "libneural\NeuralNetwork.h"
#include "libneural\TrainingSet.h"

class SupervisorThread : public QThread
{
	Q_OBJECT

public:
	SupervisorThread(QObject *parent = 0);
	~SupervisorThread();

	void run();
	void stop();
	void setStopped(bool stopped);
	void setNeuralNetwork(NeuralNetwork &net);
	void setTrainingSet(TrainingSet &set);

signals:
	void setEpoch(int);
	void setError(int);

private:
	bool stopped;
	int dead;
	int randomized;
	int epochs;
	double lastError;
	NeuralNetwork *net_;
	RPROPSupervisor supervisor;
};

#endif // SUPERVISORTHREAD_H
