
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
    void sendJSONtoServer();
    void validTime(QTreeWidgetItem* val);
    void validDate(QTreeWidgetItem* str);

    void setData();
    void closeEditor(QTreeWidgetItem* any);
    void otherItemWasChecked(QTreeWidgetItem* any);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cmdOpen();
    void cmdClose();
    void showGeneralParam();

    void setIdAndLastTask(QString id, QString task);


signals:
    void giveObjectToConvertInJson(QTreeWidget* any, int idUser);


private:
    Ui::BirdHouseClass ui;
    bool offChanger = false;
    bool sortBool = true;

    int middleColumn = 0;
    int countRow = 1;

    QTreeWidgetItem* middleItem = nullptr;
    QStatusBar* sBar;

    QString line;

    int countOfTopItems = 0;

    QSystemTrayIcon* trayIcon = nullptr;
    QDateTime fullTimeWork;

    bool windowShow = false;

    GeneralParam* myGenParam = nullptr;

    int lastNumberForTask = 1;
    int idUser = 1;

}; 