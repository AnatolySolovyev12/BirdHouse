#pragma once

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ui_AuthClass.h"

class AuthClass : public QMainWindow
{
	Q_OBJECT

public:
	AuthClass(QWidget *parent = nullptr);
	~AuthClass();

	void sendSignalToShowBird();
	void logginFunc();


signals:
	void showBirdMainWindows();
	void verifySignal(QByteArray jdoc, QString serverAdress, quint16 serverPort);

private:
	Ui::AuthClassClass ui;
};

