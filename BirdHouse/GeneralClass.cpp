#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent), birdHouseClass(new BirdHouse), tcpSocketClass(new TcpSocketClass), reFormatJsonClass(new JsonFormatingClass)
{
	birdHouseClass->show();
	connect(birdHouseClass, &BirdHouse::giveObjectToConvertInJson, reFormatJsonClass, &JsonFormatingClass::reFormat);
		
}

GeneralClass::~GeneralClass()
{}

