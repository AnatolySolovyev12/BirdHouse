#include "BirdHouse.h"

BirdHouse::BirdHouse(QWidget* parent)
	: QMainWindow(parent), sBar(new QStatusBar()), myGenParam(new GeneralParam)
{
	ui.setupUi(this);

	ui.pushButtonSetting->setEnabled(false);

	this->setWindowTitle("BirdHouse by Solovev");
	this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "\\icon.png"));

	connect(ui.pushButtonSetting, &QPushButton::clicked, this, &BirdHouse::showGeneralParam);

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

	connect(ui.pushButtonRefresh, &QPushButton::clicked, this, &BirdHouse::sendJSONtoServer);

	QMainWindow::setStatusBar(sBar);

	myGenParam->setWindowIcon(QIcon("iconParam.png"));
}



void BirdHouse::addItemInList()
{
	QTreeWidgetItem* any = nullptr;

	if (ui.treeWidget->currentItem() == nullptr)
	{
		any = new QTreeWidgetItem(ui.treeWidget);
	}
	else
	{
		if (ui.treeWidget->currentItem()->parent() == nullptr)
			any = new QTreeWidgetItem(ui.treeWidget->currentItem());
		else
			return;
	}

	int column = ui.treeWidget->currentColumn();

	offChanger = true;

	any->setText(0, any->parent() != nullptr ? QString::number(any->parent()->indexOfChild(any) + 1) : QString::number(lastNumberForTask + ui.treeWidget->indexOfTopLevelItem(any)));
	any->setText(1, "");
	any->setText(2, "");

	any->setBackground(0, QColor(221, 221, 221, 255));
	any->setBackground(1, any->parent() != nullptr ? QColor(245, 216, 183, 255) : QColor(232, 232, 232, 255));
	any->setBackground(2, any->parent() != nullptr ? QColor(217, 225, 187, 255) : QColor(213, 213, 213, 255));
	any->setCheckState(3, any->parent() != nullptr ? any->parent()->checkState(3) : any->checkState(3));
	any->setCheckState(4, any->parent() != nullptr ? any->parent()->checkState(4) : any->checkState(4));
	any->setCheckState(5, any->parent() != nullptr ? any->parent()->checkState(5) : any->checkState(5));
	any->setCheckState(6, any->parent() != nullptr ? any->parent()->checkState(6) : any->checkState(6));
	any->setText(7, any->parent() != nullptr ? any->parent()->text(7) : "");
	any->setText(8, any->parent() != nullptr ? any->parent()->text(8) : "");
	any->setText(9, any->parent() != nullptr ? any->parent()->text(9) : "");

	any->setBackground(7, QColor(119, 168, 142, 255));
	any->setBackground(8, QColor(79, 168, 142, 255));
	any->setBackground(9, QColor(88, 122, 111, 255));

	offChanger = false;

	any = nullptr;
}



void BirdHouse::deleteItemInList()
{
	if (ui.treeWidget->currentItem() == nullptr) return;

	QTreeWidgetItem* taked = ui.treeWidget->currentItem();
	QTreeWidgetItem* temp = nullptr;
	QTreeWidgetItem* parent = taked->parent();

	if (taked->parent() == nullptr)
	{
		ui.treeWidget->takeTopLevelItem(ui.treeWidget->indexOfTopLevelItem(taked));

		for (int countChild = 0; countChild < ui.treeWidget->topLevelItemCount(); countChild++)
		{
			ui.treeWidget->topLevelItem(countChild)->setText(0, QString::number(lastNumberForTask + countChild));
		}
	}
	else
	{
		parent->takeChild(parent->indexOfChild(taked));

		if (parent->childCount() == 0) return;

		for (int countChild = 0; countChild < parent->childCount(); countChild++)
		{
			temp = parent->child(countChild);
			temp->setText(0, QString::number(parent->indexOfChild(temp) + 1));
		}
	}
}



void BirdHouse::setData() // в случае двойного клика в ячейку открываем редактор
{
	QTreeWidgetItem* any = ui.treeWidget->currentItem(); // присваиваем указателю выбранную ячейку
	int column = ui.treeWidget->currentColumn(); // присваиваем переменной номер текущего столбца (отсчёт начинается с 0-ого)

	if (column == 0 || column == 3 || column == 4 || column == 5 || column == 6 || (any->parent() == nullptr ? (column == 1 || column == 2) : column == 0)) return; // не даём редактировать дальше третьего столбца            

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



void BirdHouse::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::ActivationReason::DoubleClick)
	{
		if (!windowShow && userIsAuth)
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



void BirdHouse::showGeneralParam()
{
	myGenParam->show();
}



void BirdHouse::sendJSONtoServer()
{
	ui.pushButtonAdd->setEnabled(false);
	ui.pushButtonAddMinus->setEnabled(false);
	ui.pushButtonRefresh->setEnabled(false);
	ui.pushButtonSetting->setEnabled(false);
	emit giveObjectToConvertInJson(ui.treeWidget, idUser);
}



void BirdHouse::setIdAndLastTask(QString id, QString task)
{
	idUser = id.toInt();
	lastNumberForTask = (task.toInt() + 1);
}



void BirdHouse::updateTasks()
{
	int countOfTop = ui.treeWidget->invisibleRootItem()->childCount();
	lastNumberForTask = ui.treeWidget->topLevelItem(countOfTop - 1)->text(0).toInt();
	lastNumberForTask++;

	for (int countChild = 0; countChild < ui.treeWidget->topLevelItemCount(); countChild++)
	{
		ui.treeWidget->topLevelItem(countChild)->setText(0, QString::number(lastNumberForTask + countChild));
	}

	QTimer::singleShot(3000, [this](){
		ui.pushButtonAdd->setEnabled(true);
		ui.pushButtonAddMinus->setEnabled(true);
		ui.pushButtonRefresh->setEnabled(true);
		ui.pushButtonSetting->setEnabled(true);
		});
};



void BirdHouse::setBoolAuthTrue()
{
	userIsAuth = true;
}



