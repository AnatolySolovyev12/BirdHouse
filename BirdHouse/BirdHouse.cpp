#include "BirdHouse.h"

BirdHouse::BirdHouse(QWidget * parent)
	: QMainWindow(parent), sBar(new QStatusBar()), myGenParam(new GeneralParam)
{
	ui.setupUi(this);

	connect(ui.pushButtonSetting, &QPushButton::clicked, this, &BirdHouse::showGeneralParam);
	connect(myGenParam, &GeneralParam::refreshSetting, this, &BirdHouse::refreshSettingInBirdHouse);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon("icon.png"));

	QMenu* menu = new QMenu(this);
	QAction* restoreAction = menu->addAction("CMD open and connect");
	QAction* restoreActionHide = menu->addAction("CMD disconnect");
	QAction* quitAction = menu->addAction("Exit");

	connect(restoreAction, &QAction::triggered, this, &BirdHouse::cmdOpen);
	connect(restoreActionHide, &QAction::triggered, this, &BirdHouse::cmdClose);
	connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

	trayIcon->setContextMenu(menu);
	trayIcon->setVisible(true);

	connect(trayIcon, &QSystemTrayIcon::activated, this, &BirdHouse::iconActivated);

	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &BirdHouse::addItemInList);
	connect(ui.pushButtonAddMinus, &QPushButton::clicked, this, &BirdHouse::deleteItemInList);

	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &BirdHouse::setData);
	connect(ui.treeWidget, &QTreeWidget::itemChanged, this, &BirdHouse::closeEditor);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &BirdHouse::otherItemWasChecked);

	connect(ui.pushButtonRefresh, &QPushButton::clicked, this, &BirdHouse::initializationPoolFunc);

	QMainWindow::setStatusBar(sBar);

	myGenParam->setWindowIcon(QIcon("iconParam.png"));

	refreshSettingInBirdHouse();
	initializationPoolFunc();

	getTokenFromFile();

}


void BirdHouse::addItemInList()
{
	QTreeWidgetItem* any = new QTreeWidgetItem();

	ui.treeWidget->addTopLevelItem(any);

	countOfTopItems = ui.treeWidget->topLevelItemCount();

	int column = ui.treeWidget->currentColumn();

	offChanger = true;

	any->setText(0, mBird_number);
	any->setText(1, mBird_mail);
	any->setText(2, mBird_phone);

	any->setBackground(0, QColor(221, 221, 221, 255));
	any->setBackground(1, QColor(245, 216, 183, 255));
	any->setBackground(2, QColor(217, 225, 187, 255));
	any->setCheckState(3, any->checkState(3));
	any->setCheckState(4, any->checkState(4));

	any->setCheckState(5, any->checkState(5));
	any->setCheckState(6, any->checkState(6));
	any->setText(7, mBird_date);
	any->setText(8, mBird_time);
	any->setText(9, mBird_text);

	any->setBackground(7, QColor(119, 168, 142, 255));
	any->setBackground(8, QColor(79, 168, 142, 255));
	any->setBackground(9, QColor(88, 122, 111, 255));

	offChanger = false;

	any = nullptr;
}


void BirdHouse::deleteItemInList()
{
	if (ui.treeWidget->currentItem() == nullptr) return;
	countOfTopItems = ui.treeWidget->topLevelItemCount();

	ui.treeWidget->takeTopLevelItem(ui.treeWidget->indexOfTopLevelItem(ui.treeWidget->currentItem()));
}


void BirdHouse::setData() // в случае двойного клика в ячейку открываем редактор
{
	QTreeWidgetItem* any = ui.treeWidget->currentItem(); // присваиваем указателю выбранную ячейку
	int column = ui.treeWidget->currentColumn(); // присваиваем переменной номер текущего столбца (отсчёт начинается с 0-ого)

	if (column == 0 || column == 3 || column == 4 || column == 5 || column == 6) return; // не даём редактировать дальше третьего столбца            

	middleColumn = column;
	middleItem = any;

	ui.treeWidget->openPersistentEditor(any, column); // разрешаем редактирование ячейки
	ui.treeWidget->editItem(any, column); // открываем редактор
}


void BirdHouse::closeEditor(QTreeWidgetItem* any) // слот закрытия редактора в случае внесения изменений (после нажатия Enter)
{
	if (offChanger) return; // препятствуем многократному исполнению этой функции при изменении цветов

	QString temporary = any->text(1).trimmed(); // убираем пробелы
	any->setText(1, temporary);

	temporary = any->text(2).trimmed(); // убираем пробелы
	any->setText(2, temporary);

	temporary = any->text(7).trimmed(); // убираем пробелы
	any->setText(7, temporary);

	temporary = any->text(8).trimmed(); // убираем пробелы
	any->setText(8, temporary);

	temporary = any->text(9).trimmed(); // убираем пробелы
	any->setText(9, temporary);

	offChanger = true;

	any->setCheckState(3, any->checkState(3));
	any->setCheckState(4, any->checkState(4));
	any->setCheckState(5, any->checkState(5));
	any->setCheckState(6, any->checkState(6));

	if (any->text(1).length() < 5 || any->text(1).length() > 40)
	{
		any->setText(1, "");
	}

	if (any->text(2).length() < 11 || any->text(2).length() > 12)
	{
		any->setText(2, "");
	}

	if (any->checkState(3) == Qt::Unchecked)
	{
		any->setBackground(3, QColor("white"));
	}
	else
	{
		any->setBackground(3, QColor(128, 243, 150, 255));
	}

	if (any->checkState(4) == Qt::Unchecked)
	{
		any->setBackground(4, QColor("white"));
	}
	else
	{
		any->setBackground(4, QColor(128, 243, 150, 255));
	}

	if (any->checkState(5) == Qt::Unchecked)
	{
		any->setBackground(5, QColor("white"));
	}
	else
	{
		any->setBackground(5, QColor(128, 243, 150, 255));
	}

	if (any->checkState(6) == Qt::Unchecked)
	{
		any->setBackground(6, QColor("white"));
	}
	else
	{
		any->setBackground(6, QColor(128, 243, 150, 255));
	}

	if (!any->text(7).isEmpty())
	{
		validDate(any);
	}

	if (!any->text(8).isEmpty())
	{
		validTime(any);
	}

	if ((any->text(9).length() < 5 && (!any->text(1).isEmpty() || !any->text(2).isEmpty())) || (any->text(9).length() > 150 && (!any->text(1).isEmpty() || !any->text(2).isEmpty())))
	{
		any->setText(9, "Обратитесь в Горэлектросеть касательно будущего отключения электроэнергии");
	}

	offChanger = false;

	ui.treeWidget->closePersistentEditor(middleItem, middleColumn); // закрываем редактор
}


void BirdHouse::otherItemWasChecked(QTreeWidgetItem* any) // закрываем открытый редактор в случае переключения на другой элемент
{
	if (offChanger) return;

	int column = ui.treeWidget->currentColumn();

	if (any == middleItem && column == middleColumn)
		return;

	QString temporary = any->text(0).trimmed(); // убираем пробелы
	any->setText(0, temporary);

	temporary = any->text(1).trimmed(); // убираем пробелы
	any->setText(1, temporary);

	temporary = any->text(2).trimmed(); // убираем пробелы
	any->setText(2, temporary);

	temporary = any->text(7).trimmed(); // убираем пробелы
	any->setText(7, temporary);

	temporary = any->text(8).trimmed(); // убираем пробелы
	any->setText(8, temporary);

	temporary = any->text(9).trimmed(); // убираем пробелы
	any->setText(9, temporary);

	ui.treeWidget->closePersistentEditor(middleItem, middleColumn);
	middleItem = nullptr;
}



void BirdHouse::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		ui.treeWidget->setCurrentItem(ui.treeWidget->invisibleRootItem());
	}
}



void BirdHouse::initializationPoolFunc()
{
	countOfTopItems = ui.treeWidget->topLevelItemCount();

	for (int count = 0; count < countOfTopItems; count++)
	{
		if ((ui.treeWidget->topLevelItem(count)->text(1).toStdString().length() > 40) || (ui.treeWidget->topLevelItem(count)->text(1).toStdString().length() < 5))
		{
			ui.treeWidget->topLevelItem(count)->setText(1, "");
		}

		if (ui.treeWidget->topLevelItem(count)->text(2).toInt() < 11 || ui.treeWidget->topLevelItem(count)->text(2).toInt() > 12)
		{
			ui.treeWidget->topLevelItem(count)->setText(2, "");
		}

		if (!ui.treeWidget->topLevelItem(count)->text(7).isEmpty())
		{
			validDate(ui.treeWidget->topLevelItem(count));
		}

		if (!ui.treeWidget->topLevelItem(count)->text(8).isEmpty())
		{
			validTime(ui.treeWidget->topLevelItem(count));
		}

		if ((ui.treeWidget->topLevelItem(count)->text(9).length() < 5 && (!ui.treeWidget->topLevelItem(count)->text(1).isEmpty() || !ui.treeWidget->topLevelItem(count)->text(2).isEmpty())) || ui.treeWidget->topLevelItem(count)->text(9).length() > 150 && (!ui.treeWidget->topLevelItem(count)->text(1).isEmpty() || !ui.treeWidget->topLevelItem(count)->text(2).isEmpty()))
		{
			ui.treeWidget->topLevelItem(count)->setText(9, "Обратитесь в Горэлектросеть касательно будущего отключения электроэнергии");
		}
	}
}

void BirdHouse::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::ActivationReason::DoubleClick)
	{
		if (!windowShow)
		{
			this->show();
			windowShow = true;
		}
		else
		{
			this->hide();
			windowShow = false;
		}
	}
}


void BirdHouse::cmdOpen()
{
	AllocConsole(); // Создаем консоль и присоединяем к ней текущий процесс
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout); // Перенаправляем стандартный вывод
	freopen_s(&stream, "CONOUT$", "w", stderr); // Перенаправляем стандартный вывод ошибок

	printf("\nOpen console for logout\n\n");
}



void BirdHouse::cmdClose()
{
	qDebug() << "\nProgramm disconnect from console.";

	FreeConsole(); // Отделяем процесс от cmd. После cmd закрываем руками.
}



void BirdHouse::validTime(QTreeWidgetItem* str)
{
	if (str->text(8).length() != 8)
	{
		str->setText(8, "07:00:00");
		return;
	}

	if (QTime::fromString(str->text(8), "hh:mm:ss").toString().isEmpty())
	{
		str->setText(8, "07:00:00");
		return;
	}
}



void BirdHouse::validDate(QTreeWidgetItem* str)
{
	if (str->text(7).length() != 10)
	{
		str->setText(7, QDate::currentDate().toString("yyyy-MM-dd"));
		return;
	}

	if (QDate::fromString(str->text(7), "yyyy-MM-dd").toString().isEmpty())
	{
		str->setText(7, QDate::currentDate().toString("yyyy-MM-dd"));
		return;
	}
}



void BirdHouse::getTokenFromFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\token.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		sBar->showMessage("Don't find browse file. Add a directory with a token (token.txt).", 10000);
		return;
	}

	QTextStream out(&file);

	QString myLine = out.readLine(); // метод readLine() считывает одну строку из потока

	if (myLine == "")
	{
		sBar->showMessage("Don't find browse file. Add a directory with a token (token.txt).", 10000);
		file.close();
		return;
	}

	file.close();
}



void BirdHouse::showGeneralParam()
{
	myGenParam->show();
}



void BirdHouse::refreshSettingInBirdHouse()
{
	mBird_number = myGenParam->m_name;
	mBird_mail = myGenParam->m_directory;
	mBird_phone = myGenParam->m_deadlineLine;
	mBird_timeLine = myGenParam->m_timeLine;
	mBird_rowLine = myGenParam->m_rowLine;
	mBird_date = myGenParam->m_columnLine;
	mBird_time = myGenParam->m_telegramLine;
	mBird_text = myGenParam->m_list;
	mBird_rowHead = myGenParam->m_rowHead;
}