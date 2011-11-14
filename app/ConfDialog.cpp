#include "ConfDialog.h"
#include "Layer.h"
#include <QComboBox>
#include <QColor>

ConfDialog::ConfDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteRow()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(setProperties()));
}

ConfDialog::~ConfDialog()
{
}

void ConfDialog::addRow()
{
	int rows = ui.tableWidget->rowCount();
	QTableWidgetItem *empty = new QTableWidgetItem("");
	QComboBox *combo = new QComboBox;
	combo->addItem("Unipolar");
	combo->addItem("Bipolar");
	combo->addItem("Linear");
	combo->setCurrentIndex(2);
	combo->setEnabled(false);

	if(rows == 0) {
		ui.tableWidget->insertRow(rows);
		QTableWidgetItem *typeInput = new QTableWidgetItem("Input");
		typeInput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		ui.tableWidget->setItem(rows, 1, typeInput);
		combo->setCurrentIndex(0);
		combo->setEnabled(true);
		ui.tableWidget->setCellWidget(rows, 2, combo);
	}
	else {
		ui.tableWidget->insertRow(rows);
		QTableWidgetItem *typeOutput = new QTableWidgetItem("Output");
		typeOutput->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		ui.tableWidget->setItem(rows, 1, typeOutput);
		ui.tableWidget->setCellWidget(rows, 2, combo);
		if(rows > 1) {
			ui.tableWidget->item(rows - 1, 1)->setText("Hidden");
			((QComboBox *) ui.tableWidget->cellWidget(rows - 1, 2))->setCurrentIndex(0);
			((QComboBox *) ui.tableWidget->cellWidget(rows - 1, 2))->setEnabled(true);
		}
	}
	ui.tableWidget->setItem(rows, 0, empty);
}

void ConfDialog::deleteRow()
{
	int selected = ui.tableWidget->currentRow();
	int rows = ui.tableWidget->rowCount();
	if(rows > 1) {
		if(selected == (rows - 1) && rows > 2) {
			ui.tableWidget->item(selected - 1, 1)->setText("Output");
			((QComboBox *) ui.tableWidget->cellWidget(selected - 1, 2))->setCurrentIndex(2);
			((QComboBox *) ui.tableWidget->cellWidget(selected - 1, 2))->setEnabled(false);
		}
		else if(selected == 0) {
			ui.tableWidget->item(selected + 1, 1)->setText("Input");
			((QComboBox *) ui.tableWidget->cellWidget(selected + 1, 2))->setCurrentIndex(0);
			((QComboBox *) ui.tableWidget->cellWidget(selected + 1, 2))->setEnabled(true);
		}
	}
	ui.tableWidget->removeRow(selected);
}

void ConfDialog::setProperties()
{
	properties.a = ui.aEdit->text().toInt();
	properties.b = ui.bEdit->text().toInt();
	properties.nMax = ui.nMaxEdit->text().toInt();
	properties.nMin = ui.nMinEdit->text().toInt();

	if(ui.tableWidget->rowCount() < 2)
		return;

	bool badField = false;
	for(int i = 0; i < ui.tableWidget->rowCount(); ++i) {
		ui.tableWidget->item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
		if(ui.tableWidget->item(i, 0)->text().isEmpty() || ui.tableWidget->item(i, 0)->text().toInt() < 1) {
			ui.tableWidget->item(i, 0)->setBackgroundColor(QColor(255, 0, 0));
			badField = true;
		}
	}

	if(badField)
		return;

	for(int i = 0; i < ui.tableWidget->rowCount(); ++i) {
		int neurons = ui.tableWidget->item(i, 0)->text().toInt();
		int function =  ((QComboBox *) ui.tableWidget->cellWidget(i, 2))->currentIndex();
		Layer layer(neurons, function);
		properties.layers.push_back(layer);
	}

	emit okClicked();
	hide();
}

NetworkProperties ConfDialog::getProperties()
{
	return properties;
}