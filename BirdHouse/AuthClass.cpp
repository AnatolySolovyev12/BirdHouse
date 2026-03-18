#include "AuthClass.h"

AuthClass::AuthClass(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.enterButton, &QPushButton::clicked, this, &AuthClass::logginFunc);
}

AuthClass::~AuthClass()
{
}



void AuthClass::logginFunc()
{
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




