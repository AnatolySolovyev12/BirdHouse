#pragma once

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeWidgetItem>

#include <iostream>

class JsonFormatingClass  : public QObject
{
	Q_OBJECT

public:
	JsonFormatingClass(QObject *parent = nullptr);
	~JsonFormatingClass();

	void reFormat(QTreeWidget* val);

signals:
	void sendJsonToServer(QByteArray QByteArray);

};

