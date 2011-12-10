#ifndef SUPERVISORTHREAD_H
#define SUPERVISORTHREAD_H

#include <QThread>
#include <QImage>
#include "libneural\RPROPSupervisor.h"
#include "libneural\NeuralNetwork.h"
#include "libneural\TrainingSet.h"
#include "MedQueue.h"

#define NORMALIZE_DATA

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

	void setA(double a);
	void setB(double b);
	void setNMax(double nMax);
	void setNMin(double nMin);

signals:
	void setEpoch(int, double);
	void setError(double);
	void setImage(QImage *);
	void finished();

private:
	bool stopped;
	int dead;
	int randomized;
	int epochs;
	double minError;
	NeuralNetwork *net_;
	RPROPSupervisor supervisor;
	MedQueue *queue;
	const QImage *image_;
};

#endif // SUPERVISORTHREAD_H
