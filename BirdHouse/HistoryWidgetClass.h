#pragma once

#include <QMainWindow>
#include "ui_HistoryWidgetClass.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class HistoryWidgetClass : public QMainWindow
{
	Q_OBJECT

public:
	HistoryWidgetClass(int idUser, QWidget *parent = nullptr);
	~HistoryWidgetClass();

	void getHistoryFunc();
	void showResultFromServer(QJsonDocument temp);

signals:
	void getHistorySignals(QString firstDate, QString secondDate, int idUser, bool onlyUserCheckBox);

private:
	Ui::HistoryWidgetClassClass ui;
	int m_idUser = 1;
};

