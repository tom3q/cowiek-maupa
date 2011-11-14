#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>
#include <vector>
#include "ui_MainWindow.h"
#include "ConfDialog.h"
#include "SupervisorThread.h"
#include "libneural\NeuralNetwork.h"
#include "libneural\TrainingSet.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

private slots:
	void showProperties();
	void createNetwork();
	void loadPicture();
	void play();
	void step();
	void pause();
	void stop();
	void setEpoch(int n);
	void setError(int n);

private:
	Ui::MainWindowClass ui;
	QGraphicsScene *originalScene;
	QGraphicsScene *restoredScene;
	ConfDialog *properties;
	SupervisorThread *thread;

	NeuralNetwork *net;
	TrainingSet *trainingSet;

	void init();
	void convertToGrayscale(QImage *image);
	void prepareTrainingSet(QImage *image);
};

#endif // MAINWINDOW_H
