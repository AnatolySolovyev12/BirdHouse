#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent), birdHouseClass(new BirdHouse)
{
	birdHouseClass->show();
}

GeneralClass::~GeneralClass()
{}

