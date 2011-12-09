#include "SupervisorThread.h"
#include <QImage>
#include <QColor>
#include <limits>

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
#ifdef NORMALIZE_DATA
	const double xScale = 1.0f / image_->width();
	const double yScale = 1.0f / image_->height();
#else
	const double xScale = 1.0f;
	const double yScale = 1.0f;
#endif
	const double outMin = 0.0f;
	const double outMax = 255.0f;
	for (int y = 0; y < image_->height(); ++y) {
		for (int x = 0; x < image_->width(); ++x) {
			double err, maxErr;

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
			maxErr = max(outMax - gray, gray);
			err = tmp - gray;

			errSum += (err*err)/(maxErr*maxErr);
#else
			if (fabs(tmp - gray) / gray >= 0.05f)
				++errSum;
#endif
			img->setPixel(x, y, qRgb(tmp, tmp, tmp));
		}
	}

	emit setImage(img);

#ifdef NORMALIZE_DATA
	return errSum * xScale * yScale;
#else
	return errSum / (image_->width()*image_->height());
#endif
}

void SupervisorThread::run()
{
	const double errorDelta = 0.001, deadCount = 100;
	double error;

	do {
		supervisor.train();

		error = getImageAndError();
		emit setError(error);
		if (error < 0.001)
			break;

		if (minError > error) {
			minError = error;
			dead = 0;
		}

		if (dead == deadCount) {
			minError = std::numeric_limits<double>::infinity();
			net_->randomizeConnections(5.0f);
			dead = 0;
			epochs = -1;
			randomized++;
		}

		++epochs;
		emit setEpoch(epochs, 100 - dead, minError);
		++dead;
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