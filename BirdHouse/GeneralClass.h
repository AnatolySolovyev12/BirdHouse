#pragma once

#include <QObject>
#include "BirdHouse.h"

class GeneralClass  : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject* parent = nullptr);
	~GeneralClass();


private:
	BirdHouse* birdHouseClass = nullptr;
};

