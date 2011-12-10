#include "SupervisorThread.h"
#include <QImage>
#include <QColor>
#include <limits>
#include "MedQueue.h"

SupervisorThread::SupervisorThread(const QImage &image, QObject *parent) :
	QThread(parent), image_(&image)
{
	stopped = true;
	setTerminationEnabled(true);
	queue = new MedQueue(10, 50);
	init();
}

SupervisorThread::~SupervisorThread()
{
	delete queue;
}

void SupervisorThread::init()
{
	epochs = 0;
	minError = std::numeric_limits<double>::infinity();
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
	QImage *img = new QImage(image_->width(), image_->height(), QImage::Format_RGB32);
	std::vector<double> in(2, 0), out;
	double errSum, maxErrSum;

	errSum = 0;

	const double xScale = 1.0f / image_->width();
	const double yScale = 1.0f / image_->height();
	const double outMin = 0.0f;
	const double outMax = 255.0f;

	for (int y = 0; y < image_->height(); ++y) {
		for (int x = 0; x < image_->width(); ++x) {
			double err;

			in[0] = x * xScale;
			in[1] = y * yScale;
			out = net_->getOutput(in);

			double tmp = out[0];
			if (tmp > outMax)
				tmp = outMax;
			if (tmp < outMin)
				tmp = outMin;

			QColor pix(image_->pixel(x, y));

			double gray = pix.red();
#if 1
			err = fabs(tmp - gray);
			if (err > 127.0f)
				err = 127.0f;
			errSum += err / 127.0f;
#else
			if (fabs(tmp - gray) / gray >= 0.05f)
				++errSum;
#endif
			img->setPixel(x, y, qRgb(tmp, tmp, tmp));
		}
	}

	emit setImage(img);
	return errSum * xScale * yScale;
}

void SupervisorThread::run()
{
	const double errorDelta = 0.002, errorEps = 0.10;
	double error;

	do {
		epochs++;

		supervisor.train();
		error = getImageAndError();
		emit setError(error);

		if (error < errorEps)
			break;

		queue->push(error);
		if (queue->check(errorDelta)) {
			queue->clear();
			net_->randomizeConnections(5.0f);
			epochs = 0;
			randomized++;
		}

		emit setEpoch(epochs, queue->median());
	}
	while(!stopped);

	emit finished();
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
	net_->randomizeConnections(5.0f);
	supervisor.setNeuralNetwork(net);
}

void SupervisorThread::setTrainingSet(TrainingSet &set)
{
	supervisor.setTrainingSet(set);
}

void SupervisorThread::setA(double a)
{
	supervisor.setA(a);
}

void SupervisorThread::setB(double b)
{
	supervisor.setB(b);
}

void SupervisorThread::setNMax(double nMax)
{
	supervisor.setNMax(nMax);
}

void SupervisorThread::setNMin(double nMin)
{
	supervisor.setNMin(nMin);
}