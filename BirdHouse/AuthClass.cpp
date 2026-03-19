#include "AuthClass.h"

AuthClass::AuthClass(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.enterButton, &QPushButton::clicked, this, &AuthClass::logginFunc);

	if (sqliteFunc())
		createTable();

	readDefaulWindowParams();
}

AuthClass::~AuthClass()
{
}



void AuthClass::logginFunc()
{
	if (ui.saveCheckBox->isChecked())
		writeDefaulWindowParams();

	QJsonDocument doc;

	QJsonObject verifyObj
	{
		{ "login", ui.loginLine->text() },
		{ "password", ui.passLine->text() }
	};

	doc.setObject(verifyObj);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);

	emit verifySignal(bytes, ui.ipLine->text(), ui.portLine->text().toInt());
}



void AuthClass::sendSignalToShowBird()
{
	emit showBirdMainWindows();
}


void AuthClass::setStatusBarMessege(QString text)
{
	ui.statusBar->showMessage(text, 5000);
}



bool AuthClass::sqliteFunc()
{
	QFile tempForCheckDb;
	tempForCheckDb.setFileName("defaultParamsForAuth");
	bool tempForCreateTable = true;

	if (tempForCheckDb.exists()) tempForCreateTable = false;

	mainConnection = QSqlDatabase::addDatabase("QSQLITE", "sqlite_connection");
	mainConnection.setDatabaseName("defaultParamsForAuth");

	if (!mainConnection.open())
	{
		if (mainConnection.lastError().isValid())
		{
			qDebug() << "Error in AuthClass::sqliteFunc() when try to create/open defaultParamsForAuth. Error:\n" << mainConnection.lastError().text();
		}
		else
			qDebug() << "NOT OPEN defaultParamsForAuth";

		return false;
	}

	if (!tempForCreateTable)
		qDebug() << "OPEN SQLite file with starting params (defaultParamsForAuth)";
	else
		qDebug() << "CREATE SQLite file with starting params (defaultParamsForAuth)";


	return tempForCreateTable;
}



bool AuthClass::createTable()
{
	QSqlQuery query(mainConnection);

	QString queryString = ("CREATE TABLE authWindowParams (checkBox BOOLEAN, ip VARCHAR(15), port VARCHAR(8), login VARCHAR(30), password VARCHAR(30));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in AuthClass::createTable() when try to create authWindowParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
		}
		else
			qDebug() << "NOT CREATE authWindowParams";

		return false;
	}

	qDebug() << "CREATE authWindowParams";

	return true;
}



void AuthClass::readDefaulWindowParams()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT * FROM authWindowParams;");

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in AuthClass::readDefaulWindowParams() when get default params for window. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
		}
		else
			qDebug() << "NOT GET authWindowParams values";
	}
	else
	{
		if (query.value(0).toBool())
		{
			ui.saveCheckBox->setCheckState(Qt::Checked);
			ui.ipLine->setText(query.value(1).toString());
			ui.portLine->setText(query.value(2).toString());
			ui.loginLine->setText(query.value(3).toString());
			ui.passLine->setText(query.value(4).toString());
		};
	}
}



void AuthClass::writeDefaulWindowParams()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT * FROM authWindowParams;");

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in AuthClass::writeDefaulWindowParams() when get default params for window. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
		}
		else
		{
			qDebug() << "NOT GET authWindowParams values when wuth in server";

			query.prepare("INSERT INTO authWindowParams (checkBox, ip, port, login, password) VALUES (?, ?, ?, ?, ?);");
			query.addBindValue(ui.saveCheckBox->isChecked() ? "true" : "false");
			query.addBindValue(ui.ipLine->text());
			query.addBindValue(ui.portLine->text());
			query.addBindValue(ui.loginLine->text());
			query.addBindValue(ui.passLine->text());


			if (!query.exec())
			{
				if (query.lastError().isValid())
				{
					qDebug() << "Error in AuthClass::writeDefaulWindowParams() when try to set default params for window. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
				}
			}
		}
	}
	else
	{
		qDebug() << "UPDATE";
		query.prepare("UPDATE authWindowParams SET checkBox = ?, ip = ?, port = ?, login = ?, password = ?;");
		query.addBindValue(ui.saveCheckBox->isChecked() ? "true" : "false");
		query.addBindValue(ui.ipLine->text());
		query.addBindValue(ui.portLine->text());
		query.addBindValue(ui.loginLine->text());
		query.addBindValue(ui.passLine->text());

		if (!query.exec())
		{
			if (query.lastError().isValid())
			{
				qDebug() << "Error in AuthClass::writeDefaulWindowParams() when try to update default params for window. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			}
		}
	}
}



