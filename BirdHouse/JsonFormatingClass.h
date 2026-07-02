#pragma once

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <qtimer.h>
#include <iostream>

class JsonFormatingClass  : public QObject
{
	Q_OBJECT

public:
	JsonFormatingClass(QObject *parent = nullptr);
	~JsonFormatingClass();

	void reFormat(QTreeWidget* val, int idUser);
	void historyReformat(QString dateFirst, QString dateSecond, int iD, bool onlyYou);

signals:
	void sendJsonToServer(QByteArray QByteArray);
};

