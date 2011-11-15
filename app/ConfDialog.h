#ifndef CONFDIALOG_H
#define CONFDIALOG_H

#include <QDialog>
#include "ui_ConfDialog.h"
#include "NetworkProperties.h"

class ConfDialog : public QDialog
{
	Q_OBJECT

public:
	ConfDialog(QWidget *parent = 0);
	~ConfDialog();
	NetworkProperties getProperties();

private slots:
	void addRow();
	void deleteRow();
	void setProperties();

signals:
	void okClicked();

private:
	Ui::ConfDialog ui;
	NetworkProperties properties;

	void init();
};

#endif // CONFDIALOG_H
