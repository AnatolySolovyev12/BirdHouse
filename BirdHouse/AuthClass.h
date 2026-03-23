#pragma once

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ui_AuthClass.h"


#include <QSqlDatabase>
#include <qsqlerror>
#include <QSqlQuery>

#include <QFile>

class AuthClass : public QMainWindow
{
	Q_OBJECT

public:
	AuthClass(QWidget *parent = nullptr);
	~AuthClass();

	void sendSignalToShowBird();
	void logginFunc();
	void setStatusBarMessege(QString text);


	bool sqliteFunc();
	bool createTable();

	void readDefaulWindowParams();
	void writeDefaulWindowParams();

	void regButtonPushed();


signals:
	void showBirdMainWindows();
	void verifySignal(QByteArray jdoc, QString serverAdress, quint16 serverPort);
	void enterInRegClass();

private:
	Ui::AuthClassClass ui;
	QSqlDatabase mainConnection;

};

