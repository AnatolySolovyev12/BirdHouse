#pragma once

#include <QMainWindow>
#include "ui_HistoryWidgetClass.h"

class HistoryWidgetClass : public QMainWindow
{
	Q_OBJECT

public:
	HistoryWidgetClass(QWidget *parent = nullptr);
	~HistoryWidgetClass();

private:
	Ui::HistoryWidgetClassClass ui;
};

