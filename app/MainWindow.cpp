#include "MainWindow.h"
#include <QtGui/QFileDialog>
#include <QPixmap>
#include <QRgb>
#include <vector>
#include "NetworkProperties.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
{
	init();

	connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadPicture()));
	connect(ui.editNetworkButton, SIGNAL(clicked()), this, SLOT(editNetwork()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui.stepButton, SIGNAL(clicked()), this, SLOT(step()));
	connect(thread, SIGNAL(setEpoch(int, double)), this, SLOT(setEpoch(int, double)));
	connect(thread, SIGNAL(setError(double)), this, SLOT(setError(double)));
	connect(thread, SIGNAL(setImage(QImage *)), this, SLOT(setRestoredImage(QImage *)));
}

MainWindow::~MainWindow()
{
	thread->stop();
	thread->wait();
	delete thread;
	delete properties;
	delete originalScene;
	delete restoredScene;
	if(net)
		delete net;
	if(trainingSet)
		delete trainingSet;
	delete image;
	delete restoredImage_;
}

void MainWindow::init()
{
	ui.setupUi(this);

	image = new QImage();
	originalScene = new QGraphicsScene();
	restoredScene = new QGraphicsScene();
	ui.originalView->setScene(originalScene);
	ui.restoredView->setScene(restoredScene);
	properties = new ConfDialog();
	thread = new SupervisorThread(*image);

	net = NULL;
	trainingSet = NULL;
	restoredImage_ = NULL;
	imageLoaded = false;
	netCreated = false;
	threadReady = false;

	QString title = windowTitle();
	title.append(" ");
	title.append(VERSION);
	setWindowTitle(title);
}

void MainWindow::editNetwork()
{
	int result = properties->exec();
	if(result == QDialog::Rejected)
		return;

	if(thread->isRunning())
		stopThread();

	if(net)
		delete net;

	net = new NeuralNetwork();
	np = properties->getProperties();
	net->addInputLayer(2);
	for(QLinkedList<Layer>::const_iterator it = np.layers.begin(); it != np.layers.end(); ++it)
		net->addLayer((*it).neuronCount, (*it).activationFunction);

	setEpoch(0, 0);
	setError(0);
	thread->init();
	restoredScene->clear();
	delete restoredImage_;
	restoredImage_ = 0;

	netCreated = true;
	threadReady = false;
}

void MainWindow::loadPicture()
{
	QString filename = QFileDialog::getOpenFileName();
	if(filename.isNull())
		return;

	if(thread->isRunning())
		stopThread();

	image->load(filename);
	imageWidth_ = image->width();
	imageHeight_ = image->height();

	convertToGrayscale(image);
	prepareTrainingSet(image);
	originalScene->clear();
	originalScene->addPixmap(QPixmap::fromImage(*image));
	setEpoch(0, 0);
	setError(0);
	thread->init();
	restoredScene->clear();
	delete restoredImage_;
	restoredImage_ = 0;

	imageLoaded = true;
	threadReady = false;
}

void MainWindow::convertToGrayscale(QImage *image)
{
	for(int i = 0; i < image->width(); ++i) {
		for(int j = 0; j < image->height(); ++j) {
			QRgb rgb = image->pixel(i, j);
			unsigned int gray = qGray(rgb);
			image->setPixel(i, j, qRgb(gray, gray, gray));
		}
	}
}

void MainWindow::play()
{
	if(prepareThread())
		return;

	ui.playButton->setText("Pause");
	disconnect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(pause()));
	ui.stepButton->setEnabled(false);
	thread->setStopped(false);
	thread->start();
}

void MainWindow::step()
{
	if(prepareThread())
		return;

	thread->setStopped(true);
	thread->start();
	thread->wait();
}

void MainWindow::pause()
{
	ui.playButton->setText("Play");
	disconnect(ui.playButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	ui.stepButton->setEnabled(true);
	thread->stop();
	thread->wait();
}

void MainWindow::setEpoch(int n, double minError)
{
	ui.epochDisplay->display(n);
	ui.minimumError->setText(QString::number(minError));
}

void MainWindow::setError(double err)
{
	ui.errorDisplay->display(err);
}

void MainWindow::prepareTrainingSet(QImage *image)
{
	if(trainingSet)
		delete trainingSet;

	trainingSet = new TrainingSet();

	std::vector<double> in, out;
	in.resize(2, 0);
	out.resize(1, 0);
#ifdef NORMALIZE_DATA
	const double xScale = 1.0f / image->width();
	const double yScale = 1.0f / image->height();
#else
	const double xScale = 1.0f;
	const double yScale = 1.0f;
#endif
	trainingSet->reserve(image->width() * image->height());
	for(int i = 0; i < image->width(); i += 2) {
		for(int j = 0; j < image->height(); j += 2) {
			QRgb rgb = image->pixel(i, j);
			double gray = QColor::fromRgb(rgb).red();
			in[0] = i * xScale;
			in[1] = j * yScale;
			out[0] = gray;
			trainingSet->addData(in, out);
		}
	}
}

void MainWindow::setRestoredImage(QImage *img)
{
	delete restoredImage_;

	restoredImage_ = img;

	QPixmap pixels = QPixmap::fromImage(*img);
	restoredScene->clear();
	restoredScene->addPixmap(pixels);
}

void MainWindow::stopThread()
{
	thread->stop();
	thread->wait();

	ui.playButton->setText("Play");
	ui.stepButton->setEnabled(true);
	disconnect(ui.playButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
}

int MainWindow::prepareThread()
{
	if(!netCreated)
		editNetwork();

	if(!imageLoaded)
		loadPicture();

	if(!imageLoaded || !netCreated)
		return 1;

	if(!threadReady) {
		thread->setNeuralNetwork(*net);
		thread->setTrainingSet(*trainingSet);
		thread->setA(np.a);
		thread->setB(np.b);
		thread->setNMax(np.nMax);
		thread->setNMin(np.nMin);
	}

	return 0;
}
