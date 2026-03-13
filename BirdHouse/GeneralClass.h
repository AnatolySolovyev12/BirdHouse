#pragma once

#include <QObject>

class GeneralClass  : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject *parent);
	~GeneralClass();
};

