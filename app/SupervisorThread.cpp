#include "SupervisorThread.h"
#include <QImage>
#include <QColor>

SupervisorThread::SupervisorThread(const QImage &image, QObject *parent) :
	QThread(parent), image_(&image)
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

template<typename T>
T max(const T &a, const T &b)
{
	return (a < b) ? b : a;
}

double SupervisorThread::getImageAndError()
{
	std::vector<double> in(2, 0), out;
	double errSum, maxErrSum;

	errSum = 0;

	for (int y = 0; y < image_->height(); ++y) {
		for (int x = 0; x < image_->width(); ++x) {
			double err, maxErr;

			in[0] = x;
			in[1] = y;
			out = net_->getOutput(in);

			QColor pix(image_->pixel(x, y));
			maxErr = max(255 - pix.red(), pix.red());
			err = out[0] - pix.red();

			errSum += (fabs(err))/(maxErr);
		}
	}

	return errSum / (image_->width()*image_->height());
}

void SupervisorThread::run()
{
	const double errorDelta = 0.001, deadCount = 10;
	double error;

	do {
		supervisor.train();

		error = getImageAndError();
		if (error < 0.001)
			break;


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