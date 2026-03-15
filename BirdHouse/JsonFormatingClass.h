#pragma once

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeWidgetItem>

class JsonFormatingClass  : public QObject
{
	Q_OBJECT

public:
	JsonFormatingClass(QObject *parent = nullptr);
	~JsonFormatingClass();

	void reFormat(QTreeWidgetItem* any);
};

