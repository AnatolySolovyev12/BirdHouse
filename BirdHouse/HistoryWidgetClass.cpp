#include "HistoryWidgetClass.h"

HistoryWidgetClass::HistoryWidgetClass(int idUser, QWidget* parent)
	: QMainWindow(parent), m_idUser(idUser)
{
	ui.setupUi(this);

	connect(ui.getButton, &QPushButton::clicked, this, &HistoryWidgetClass::getHistoryFunc);
}

HistoryWidgetClass::~HistoryWidgetClass()
{}



void HistoryWidgetClass::getHistoryFunc()
{
	emit getHistorySignals(ui.dateEdit->date().toString("yyyy-MM-dd"), ui.dateEdit_2->date().toString("yyyy-MM-dd"), m_idUser, ui.checkBox->checkState());
}