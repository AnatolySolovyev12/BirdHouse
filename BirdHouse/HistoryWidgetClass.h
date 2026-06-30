#pragma once

#include <QMainWindow>
#include "ui_HistoryWidgetClass.h"

class HistoryWidgetClass : public QMainWindow
{
	Q_OBJECT

public:
	HistoryWidgetClass(int idUser, QWidget *parent = nullptr);
	~HistoryWidgetClass();

	void getHistoryFunc();

signals:
	void getHistorySignals(QString firstDate, QString secondDate, int idUser, bool onlyUserCheckBox);

private:
	Ui::HistoryWidgetClassClass ui;
	int m_idUser = 1;
};

