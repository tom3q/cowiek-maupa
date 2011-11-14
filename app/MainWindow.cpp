#include "MainWindow.h"
#include <QtGui/QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <vector>
#include "NetworkProperties.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
{
	init();

	connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadPicture()));
	connect(ui.createNetworkButton, SIGNAL(clicked()), this, SLOT(showProperties()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui.stepButton, SIGNAL(clicked()), this, SLOT(step()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	connect(properties, SIGNAL(okClicked()), this, SLOT(createNetwork()));
}

MainWindow::~MainWindow()
{
	delete properties;
	delete originalScene;
	delete restoredScene;
	if(thread)
		delete thread;
	if(net)
		delete net;
	if(trainingSet)
		delete trainingSet;
}

void MainWindow::init()
{
	ui.setupUi(this);

	originalScene = new QGraphicsScene();
	restoredScene = new QGraphicsScene();
	ui.originalView->setScene(originalScene);
	ui.restoredView->setScene(restoredScene);
	properties = new ConfDialog();

	net = NULL;
	trainingSet = NULL;
	thread = NULL;
}

void MainWindow::showProperties()
{
	properties->show();
}

void MainWindow::createNetwork()
{
	if(thread)
		delete thread;

	if(net)
		delete net;

	net = new NeuralNetwork();
	thread = new SupervisorThread();
	NetworkProperties np = properties->getProperties();
	net->addInputLayer(2);
	for(QLinkedList<Layer>::const_iterator it = np.layers.begin(); it != np.layers.end(); ++it)
		net->addLayer((*it).neuronCount, (*it).activationFunction);

	thread->setNeuralNetwork(*net);
	connect(thread, SIGNAL(setEpoch(int)), this, SLOT(setEpoch(int)));
	connect(thread, SIGNAL(setError(int)), this, SLOT(setError(int)));
	ui.loadButton->setEnabled(true);
}

void MainWindow::loadPicture()
{
	QString filename = QFileDialog::getOpenFileName();
	QImage image(filename);

	if(trainingSet)
		delete trainingSet;

	trainingSet = new TrainingSet();
	convertToGrayscale(&image);
	prepareTrainingSet(&image);
	thread->setTrainingSet(*trainingSet);
	QPixmap pixmap = QPixmap::fromImage(image);
	originalScene->clear();
	originalScene->addPixmap(pixmap);
	ui.playButton->setEnabled(true);
	ui.stepButton->setEnabled(true);
	ui.stopButton->setEnabled(true);
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
	ui.playButton->setText("Pause");
	disconnect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(pause()));
	ui.stepButton->setEnabled(false);
	ui.createNetworkButton->setEnabled(false);
	thread->setStopped(false);
	thread->start();
}

void MainWindow::step()
{
	thread->setStopped(true);
	thread->start();
}

void MainWindow::pause()
{
	ui.playButton->setText("Play");
	disconnect(ui.playButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(ui.playButton, SIGNAL(clicked()), this, SLOT(play()));
	ui.stepButton->setEnabled(true);
	ui.createNetworkButton->setEnabled(true);
	thread->stop();
}

void MainWindow::stop()
{
	thread->stop();
	setEpoch(0);
	setError(0);
	ui.playButton->setEnabled(false);
	ui.stepButton->setEnabled(false);
	ui.stopButton->setEnabled(false);
	originalScene->clear();
}

void MainWindow::setEpoch(int n)
{
	ui.epochDisplay->display(n);
}

void MainWindow::setError(int n)
{
	ui.errorDisplay->display(n);
}

void MainWindow::prepareTrainingSet(QImage *image)
{
	std::vector<double> in, out;
	in.resize(2, 0);
	out.resize(1, 0);
	trainingSet->reserve(image->width() * image->height());
	for(int i = 0; i < image->width(); ++i) {
		for(int j = 0; j < image->height(); ++j) {
			QRgb rgb = image->pixel(i, j);
			unsigned int gray = qGray(rgb);
			in[0] = i;
			in[1] = j;
			out[0] = gray;
			trainingSet->addData(in, out);
		}
	}
}