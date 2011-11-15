#include "ConfDialog.h"
#include "Layer.h"
#include <QComboBox>
#include <QColor>
#include <QPalette>

ConfDialog::ConfDialog(QWidget *parent) : QDialog(parent)
{
	init();

	connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteRow()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(setProperties()));
}

ConfDialog::~ConfDialog()
{
}

void ConfDialog::init()
{
	ui.setupUi(this);

	QComboBox *combo = new QComboBox;
	combo->addItem("Unipolar");
	combo->addItem("Bipolar");
	combo->addItem("Linear");
	combo->setCurrentIndex(2);
	combo->setEnabled(false);

	QTableWidgetItem *typeInput = new QTableWidgetItem("Input");
	QTableWidgetItem *typeOutput = new QTableWidgetItem("Output");
	QTableWidgetItem *countInput = new QTableWidgetItem("2");
	QTableWidgetItem *countOutput = new QTableWidgetItem("1");
	QTableWidgetItem *empty = new QTableWidgetItem("");
	typeInput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	typeOutput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	countInput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	countOutput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	empty->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, countInput);
	ui.tableWidget->setItem(0, 1, typeInput);
	ui.tableWidget->setItem(0, 2, empty);

	ui.tableWidget->insertRow(1);
	ui.tableWidget->setItem(1, 0, countOutput);
	ui.tableWidget->setItem(1, 1, typeOutput);
	ui.tableWidget->setCellWidget(1, 2, combo);
}

void ConfDialog::addRow()
{
	int rows = ui.tableWidget->rowCount();
	QTableWidgetItem *empty = new QTableWidgetItem("");
	QTableWidgetItem *typeHidden = new QTableWidgetItem("Hidden");
	typeHidden->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	QComboBox *combo = new QComboBox;
	combo->addItem("Unipolar");
	combo->addItem("Bipolar");
	combo->addItem("Linear");

	ui.tableWidget->insertRow(rows - 1);
	ui.tableWidget->setItem(rows - 1, 0, empty);
	ui.tableWidget->setItem(rows - 1, 1, typeHidden);
	ui.tableWidget->setCellWidget(rows - 1, 2, combo);
}

void ConfDialog::deleteRow()
{
	int selected = ui.tableWidget->currentRow();
	int rows = ui.tableWidget->rowCount();
	if(selected == 0 || selected == (rows - 1))
		return;

	ui.tableWidget->removeRow(selected);
}

void ConfDialog::setProperties()
{
	bool badField = false;
	for(int i = 0; i < ui.tableWidget->rowCount(); ++i) {
		ui.tableWidget->item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
		if(ui.tableWidget->item(i, 0)->text().isEmpty() || ui.tableWidget->item(i, 0)->text().toInt() < 1) {
			ui.tableWidget->item(i, 0)->setBackgroundColor(QColor(255, 0, 0));
			badField = true;
		}
	}

	QPalette p = ui.aEdit->palette();
	p.setColor(QPalette::Base, QColor(255,255,255));
	ui.aEdit->setPalette(p);
	if(ui.aEdit->text().isEmpty() || ui.aEdit->text().toInt() <= 0) {
		QPalette p = ui.aEdit->palette();
		p.setColor(QPalette::Base, QColor(255,0,0));
		ui.aEdit->setPalette(p);
		badField = true;
	}
	p = ui.bEdit->palette();
	p.setColor(QPalette::Base, QColor(255,255,255));
	ui.bEdit->setPalette(p);
	if(ui.bEdit->text().isEmpty() || ui.bEdit->text().toInt() <= 0) {
		p = ui.bEdit->palette();
		p.setColor(QPalette::Base, QColor(255,0,0));
		ui.bEdit->setPalette(p);
		badField = true;
	}
	p = ui.nMaxEdit->palette();
	p.setColor(QPalette::Base, QColor(255,255,255));
	ui.nMaxEdit->setPalette(p);
	if(ui.nMaxEdit->text().isEmpty() || ui.nMaxEdit->text().toInt() <= 0) {
		p = ui.nMaxEdit->palette();
		p.setColor(QPalette::Base, QColor(255,0,0));
		ui.nMaxEdit->setPalette(p);
		badField = true;
	}
	p = ui.nMinEdit->palette();
	p.setColor(QPalette::Base, QColor(255,255,255));
	ui.nMinEdit->setPalette(p);
	if(ui.nMinEdit->text().isEmpty() || ui.nMinEdit->text().toInt() <= 0) {
		p = ui.nMinEdit->palette();
		p.setColor(QPalette::Base, QColor(255,0,0));
		ui.nMinEdit->setPalette(p);
		badField = true;
	}

	if(badField)
		return;

	properties.a = ui.aEdit->text().toInt();
	properties.b = ui.bEdit->text().toInt();
	properties.nMax = ui.nMaxEdit->text().toInt();
	properties.nMin = ui.nMinEdit->text().toInt();

	for(int i = 1; i < ui.tableWidget->rowCount(); ++i) {
		int neurons = ui.tableWidget->item(i, 0)->text().toInt();
		int function =  ((QComboBox *) ui.tableWidget->cellWidget(i, 2))->currentIndex();
		Layer layer(neurons, function);
		properties.layers.push_back(layer);
	}

	accept();
}

NetworkProperties ConfDialog::getProperties()
{
	return properties;
}