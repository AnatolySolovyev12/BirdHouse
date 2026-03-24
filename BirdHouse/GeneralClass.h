#pragma once

#include <QObject>
#include "BirdHouse.h"
#include "TcpSocketClass.h"
#include "JsonFormatingClass.h"
#include "AuthClass.h"
#include "RegClass.h"

class GeneralClass : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject* parent = nullptr);
	~GeneralClass();

	void showBirdWindow();

signals:
	void userAndTask(QString id, QString task);
	void setIpAndPortForRegClass(QString ip, QString port);


private:
	BirdHouse* birdHouseClass = nullptr;
	TcpSocketClass* tcpSocketClass = nullptr;
	JsonFormatingClass* reFormatJsonClass = nullptr;
	AuthClass* authClass = nullptr;
	RegClass* regClass = nullptr;
};