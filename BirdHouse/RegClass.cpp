#include "RegClass.h"

RegClass::RegClass(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.exitButton, &QPushButton::clicked, this, &RegClass::exitFunc);
	connect(ui.regButton, &QPushButton::clicked, this, &RegClass::registerFunc);
	connect(ui.enterButton, &QPushButton::clicked, this, &RegClass::codeMailSendFunc);

	ui.codeLine->hide();
	ui.enterButton->hide();
	ui.codeLabel->hide();
}



RegClass::~RegClass()
{
}



void RegClass::exitFunc()
{
	emit exitSignal();

	ui.codeLine->hide();
	ui.enterButton->hide();
	ui.codeLabel->hide();

	ui.ipLabel->show();
	ui.ipLine->show();
	ui.portLabel->show();
	ui.portLine->show();
	ui.loginLabel->show();
	ui.loginLine->show();
	ui.passLabel->show();
	ui.passLine->show();
	ui.regButton->show();
}



void RegClass::registerFunc()
{
	QJsonDocument doc;

	QJsonObject verifyObj
	{
		{ "register", "true" },
		{ "login", ui.loginLine->text() },
		{ "password", ui.passLine->text() }
	};

	doc.setObject(verifyObj);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);

	emit registerSignal(bytes, ui.ipLine->text(), ui.portLine->text().toInt());
}



void RegClass::hideRegElement()
{
	ui.codeLine->show();
	ui.enterButton->show();
	ui.codeLabel->show();

	ui.ipLabel->hide();
	ui.ipLine->hide();
	ui.portLabel->hide();
	ui.portLine->hide();
	ui.loginLabel->hide();
	ui.loginLine->hide();
	ui.passLabel->hide();
	ui.passLine->hide();
	ui.regButton->hide();


	ui.statusBar->showMessage("Check password on your Email", 10000);
}



void RegClass::codeMailSendFunc()
{
	QJsonDocument doc;

	QJsonObject verifyObj
	{
		{ "CodeForReg", "true" },
		{ "login", ui.loginLine->text() },
		{ "password", ui.passLine->text() },
		{ "codeMail", ui.codeLine->text() }
	};

	doc.setObject(verifyObj);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);

	emit registerSignal(bytes, ui.ipLine->text(), ui.portLine->text().toInt());
}


