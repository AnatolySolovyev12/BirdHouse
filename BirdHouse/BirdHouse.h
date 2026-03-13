
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BirdHouse.h"

#include <QFileDialog>
#include <QFile>
#include <QMouseEvent>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include <windows.h>

#include "GeneralParam.h"

#include <QDate>

class BirdHouse : public QMainWindow
{
    Q_OBJECT

public:

    BirdHouse(QWidget* parent = nullptr);

    void addItemInList();
    void deleteItemInList();


    void mousePressEvent(QMouseEvent* event);
    void initializationPoolFunc();
    void validTime(QTreeWidgetItem* val);
    void validDate(QTreeWidgetItem* str);

    void setData();
    void closeEditor(QTreeWidgetItem* any);
    void otherItemWasChecked(QTreeWidgetItem* any);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cmdOpen();
    void cmdClose();

    void getTokenFromFile();

    void showGeneralParam();

    void refreshSettingInBirdHouse();

private:
    Ui::BirdHouseClass ui;
    bool offChanger = false;
    bool sortBool = true;

    int middleColumn = 0;
    int countRow = 1;

    QTreeWidgetItem* middleItem = nullptr;
    QStatusBar* sBar;

    QString line;

    int countOfTopItems;

    QSystemTrayIcon* trayIcon = nullptr;
    QDateTime fullTimeWork;

    bool windowShow = false;

    GeneralParam* myGenParam = nullptr;

    QString mBird_number = "new";
    QString mBird_mail = "path";
    QString mBird_phone = "15";
    QString mBird_timeLine = "07:00:00";
    QString mBird_rowLine = "1";
    QString mBird_date = "1";
    QString mBird_time;
    QString mBird_text = "1";
    QString mBird_rowHead = "1";
};