#pragma once

#include <QObject>
#include "BirdHouse.h"
#include "TcpSocketClass.h"
#include "JsonFormatingClass.h"

class GeneralClass  : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject* parent = nullptr);
	~GeneralClass();


private:
	BirdHouse* birdHouseClass = nullptr;
	TcpSocketClass* tcpSocketClass = nullptr;
	JsonFormatingClass* reFormatJsonClass = nullptr;
};

