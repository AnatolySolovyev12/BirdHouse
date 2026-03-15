#include "TcpSocketClass.h"

TcpSocketClass::TcpSocketClass(QObject *parent)
	: QObject(parent), mTcpSocket(new QTcpSocket)
{
	connect(mTcpSocket, &QTcpSocket::connected, this, &TcpSocketClass::onConnected);
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &TcpSocketClass::onDisconnected);
	connect(mTcpSocket, &QTcpSocket::readyRead, this, &TcpSocketClass::onReadyRead);
	connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &TcpSocketClass::onErrorOccurred);
}


TcpSocketClass::~TcpSocketClass()
{
	if (mTcpSocket->isOpen()) {
		mTcpSocket->close();
	}
}

void TcpSocketClass::connectToServer(QString messege)
{
	if (!mTcpSocket->isOpen())
	{
		tempMessege = QByteArray::fromStdString(messege.toStdString());
		mTcpSocket->connectToHost(QHostAddress(host), port);
		qDebug() << "Connect to " + QHostAddress(host).toString();
	}

	mTcpSocket->write(tempMessege);
}


void TcpSocketClass::onConnected()
{
	// connectedState = true;
	qDebug() << "\nConnected to server\n";
}

void TcpSocketClass::onDisconnected()
{
	connectedState = false;
	qDebug() << "\nDisconnected from server.";
}



void TcpSocketClass::onReadyRead()
{
	QByteArray data = mTcpSocket->readAll();

	qDebug() << "RX << " << data.toHex();
}


void TcpSocketClass::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
	qDebug() << "\nSocket error:" << socketError << mTcpSocket->errorString();
}
