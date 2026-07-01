#include "HistoryWidgetClass.h"

HistoryWidgetClass::HistoryWidgetClass(int idUser, QWidget* parent)
	: QMainWindow(parent), m_idUser(idUser)
{
	ui.setupUi(this);

	connect(ui.getButton, &QPushButton::clicked, this, &HistoryWidgetClass::getHistoryFunc);

	ui.tableWidget->setColumnCount(14);
	ui.tableWidget->setHorizontalHeaderLabels({ "ID пользователя","ID заявки","ID позиции", "Номер телефона", "Почта", "МАХ рассылка", "Telegram рассылка", "Email рассылка", "SMS рассылка", "Дата уведомления", "Время уведомления", "Дата создания", "Время создания", "Текст уведомления" });
}

HistoryWidgetClass::~HistoryWidgetClass()
{}



void HistoryWidgetClass::getHistoryFunc()
{
	emit getHistorySignals(ui.dateEdit->date().toString("yyyy-MM-dd"), ui.dateEdit_2->date().toString("yyyy-MM-dd"), m_idUser, ui.checkBox->checkState());



	ui.tableWidget->setRowCount(0);
}