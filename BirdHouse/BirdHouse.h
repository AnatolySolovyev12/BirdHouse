#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BirdHouse.h"

class BirdHouse : public QMainWindow
{
    Q_OBJECT

public:
    BirdHouse(QWidget *parent = nullptr);
    ~BirdHouse();

private:
    Ui::BirdHouseClass ui;
};

