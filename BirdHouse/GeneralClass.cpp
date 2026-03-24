#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent), birdHouseClass(new BirdHouse), tcpSocketClass(new TcpSocketClass), reFormatJsonClass(new JsonFormatingClass), authClass(new AuthClass), regClass(new RegClass)
{
	authClass->show();
	
	connect(birdHouseClass, &BirdHouse::giveObjectToConvertInJson, reFormatJsonClass, &JsonFormatingClass::reFormat);
	connect(reFormatJsonClass, &JsonFormatingClass::sendJsonToServer, tcpSocketClass, &TcpSocketClass::connectToServer);
	connect(authClass, &AuthClass::showBirdMainWindows, this, &GeneralClass::showBirdWindow);
	
	connect(authClass, &AuthClass::verifySignal, tcpSocketClass, [&](QByteArray jdoc, QString serverAdress, quint16 serverPort) {
		tcpSocketClass->setIpPort(serverAdress, serverPort);
		tcpSocketClass->connectToServer(jdoc);
		});

	connect(regClass, &RegClass::registerSignal, tcpSocketClass, [&](QByteArray jdoc, QString serverAdress, quint16 serverPort) {
		tcpSocketClass->setIpPort(serverAdress, serverPort);
		tcpSocketClass->connectToServer(jdoc);
		});


	connect(regClass, &RegClass::sendCodeMailSignal, tcpSocketClass, [&](QByteArray jdoc, QString serverAdress, quint16 serverPort) {
		tcpSocketClass->setIpPort(serverAdress, serverPort);
		tcpSocketClass->connectToServer(jdoc);
		});


	connect(this, &GeneralClass::setIpAndPortForRegClass, regClass, &RegClass::setLoginAndPassInUI);

	connect(authClass, &AuthClass::enterInRegClass, [this](QString ipValue, QString portValue) {
		authClass->hide();
		emit setIpAndPortForRegClass(ipValue, portValue);
		regClass->show();
		});

	connect(regClass, &RegClass::exitSignal, [this]() {
		authClass->show();
		regClass->hide();
		});

	connect(this, &GeneralClass::userAndTask, birdHouseClass, &BirdHouse::setIdAndLastTask);

	connect(tcpSocketClass, &TcpSocketClass::accessAllowed, [this](QString iduser, QString lastTask) {
		emit this->userAndTask(iduser, lastTask);
		this->showBirdWindow();
		});

	connect(tcpSocketClass, &TcpSocketClass::statusBarMessege, authClass, &AuthClass::setStatusBarMessege);
	connect(tcpSocketClass, &TcpSocketClass::updateTaskAndOnInterface, birdHouseClass, &BirdHouse::updateTasks);
	connect(tcpSocketClass, &TcpSocketClass::checkCodeInMail, regClass, &RegClass::hideRegElement);
	connect(tcpSocketClass, &TcpSocketClass::registerIsDoneSignal, regClass, &RegClass::exitFunc);
	connect(tcpSocketClass, &TcpSocketClass::statusBarRegMessege, regClass, &RegClass::statusBarMessegeForRegCLass);



}



GeneralClass::~GeneralClass()
{}



void GeneralClass::showBirdWindow()
{
	authClass->hide();
	birdHouseClass->show();
}
