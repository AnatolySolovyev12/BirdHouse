#pragma once

#include <QMainWindow>
#include "ui_RegClass.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class RegClass : public QMainWindow
{
	Q_OBJECT

public:
	RegClass(QWidget *parent = nullptr);
	~RegClass();

	void exitFunc();
	void registerFunc();
	void hideRegElement();
	void codeMailSendFunc();

signals:
	void exitSignal();
	void registerSignal(QByteArray bArray, QString serverAdress, quint16 serverPort);
	void sendCodeMailSignal(QByteArray bArray, QString serverAdress, quint16 serverPort);

private:
	Ui::RegClassClass ui;
};

