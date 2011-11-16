#ifndef SUPERVISORTHREAD_H
#define SUPERVISORTHREAD_H

#include <QThread>
#include <QImage>
#include "libneural\RPROPSupervisor.h"
#include "libneural\NeuralNetwork.h"
#include "libneural\TrainingSet.h"

class SupervisorThread : public QThread
{
	Q_OBJECT

public:
	SupervisorThread(const QImage &image, QObject *parent = 0);
	~SupervisorThread();

	void init();
	void run();
	void stop();
	void setStopped(bool stopped);
	void setNeuralNetwork(NeuralNetwork &net);
	void setTrainingSet(TrainingSet &set);
	double getImageAndError();

signals:
	void setEpoch(int);
	void setError(double);

private:
	bool stopped;
	int dead;
	int randomized;
	int epochs;
	double lastError;
	NeuralNetwork *net_;
	RPROPSupervisor supervisor;
	const QImage *image_;
};

#endif // SUPERVISORTHREAD_H
