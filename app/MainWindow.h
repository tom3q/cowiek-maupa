#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>
#include <QImage>
#include <vector>
#include "ui_MainWindow.h"
#include "ConfDialog.h"
#include "SupervisorThread.h"
#include "../libneural/NeuralNetwork.h"
#include "../libneural/TrainingSet.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

private slots:
	void editNetwork();
	void loadPicture();
	void play();
	void step();
	void pause();
	void setEpoch(int n);
	void setError(int n);

private:
	Ui::MainWindowClass ui;
	QGraphicsScene *originalScene;
	QGraphicsScene *restoredScene;
	ConfDialog *properties;
	SupervisorThread *thread;
	QImage *image;
	bool imageLoaded;
	bool netCreated;
	bool threadReady;

	NeuralNetwork *net;
	TrainingSet *trainingSet;
	NetworkProperties np;

	int imageWidth_, imageHeight_;

	void init();
	void convertToGrayscale(QImage *image);
	void prepareTrainingSet(QImage *image);
	void stopThread();

	void drawTheSHIT();
};

#endif // MAINWINDOW_H
