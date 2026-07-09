#include "HistoryWidgetClass.h"

HistoryWidgetClass::HistoryWidgetClass(int idUser, QWidget* parent)
	: QMainWindow(parent), m_idUser(idUser)
{
	ui.setupUi(this);

	this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "\\history.png"));

	connect(ui.getButton, &QPushButton::clicked, this, &HistoryWidgetClass::getHistoryFunc);

	ui.tableWidget->setColumnCount(14);
	ui.tableWidget->setHorizontalHeaderLabels({ "ID пользователя","ID заявки","ID позиции", "Номер телефона", "Почта", "МАХ рассылка", "Telegram рассылка", "Email рассылка", "SMS рассылка", "Дата уведомления", "Время уведомления", "Дата создания", "Время создания", "Текст уведомления" });
	ui.dateEdit->setDate(QDate::currentDate());
	ui.dateEdit_2->setDate(QDate::currentDate());
}

HistoryWidgetClass::~HistoryWidgetClass()
{}



void HistoryWidgetClass::getHistoryFunc()
{
	emit getHistorySignals(ui.dateEdit->date().toString("yyyy-MM-dd"), ui.dateEdit_2->date().toString("yyyy-MM-dd"), m_idUser, ui.checkBox->checkState());

	ui.tableWidget->setRowCount(0);
}



void HistoryWidgetClass::showResultFromServer(QJsonDocument temp)
{
	QJsonObject tempObj = temp.object();
	QJsonArray tempArr = tempObj["array"].toArray();

	int rowCounter = 0;

	for (QJsonValue val : tempArr)
	{
		ui.tableWidget->insertRow(rowCounter); // добавляем новую строку под определённым порядковым номером

		for (int itemCounter = 0; itemCounter < 14; itemCounter++) // заполняем новую строку значениями из Json согласно своих позиций
		{
			QTableWidgetItem* tempItem = new QTableWidgetItem();
			ui.tableWidget->setItem(rowCounter, itemCounter, tempItem);

			if (rowCounter % 2 == 0) tempItem->setBackground(QColor(176, 224, 230)); // задаём чередующийся фон для записей
			else tempItem->setBackground(QColor(220, 220, 220));

			switch (itemCounter)
			{
			case(0): {tempItem->setText(val["id_user"].toString()); break;}
			case(1): {tempItem->setText(val["id_request"].toString()); break;}
			case(2): {tempItem->setText(val["id_position"].toString()); break;}
			case(3): {tempItem->setText(val["phone_number"].toString()); break;}
			case(4): {tempItem->setText(val["mail"].toString()); break;}
			case(5): {tempItem->setText(val["max_send"].toString()); break;}
			case(6): {tempItem->setText(val["tg_send"].toString()); break;}
			case(7): {tempItem->setText(val["mail_send"].toString()); break;}
			case(8): {tempItem->setText(val["sms_send"].toString()); break;}
			case(9): {tempItem->setText(val["date_create"].toString()); break;}
			case(10): {tempItem->setText(val["time_create"].toString()); break;}
			case(11): {tempItem->setText(val["date"].toString()); break;}
			case(12): {tempItem->setText(val["time"].toString()); break;}
			case(13): {tempItem->setText(val["text"].toString()); break;}
			}
		}

		++rowCounter;
	}
}