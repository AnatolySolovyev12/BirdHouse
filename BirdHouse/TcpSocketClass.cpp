#include "TcpSocketClass.h"

TcpSocketClass::TcpSocketClass(QObject* parent)
	: QObject(parent), mTcpSocket(new QTcpSocket), timerForCheckSending(new QTimer)
{
	connect(mTcpSocket, &QTcpSocket::connected, this, &TcpSocketClass::onConnected);
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &TcpSocketClass::onDisconnected);
	connect(mTcpSocket, &QTcpSocket::readyRead, this, &TcpSocketClass::onReadyRead);
	connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &TcpSocketClass::onErrorOccurred);

	connect(timerForCheckSending, &QTimer::timeout, this, [this]() {

		resendingCounter++;

		qDebug() << "\nNOT ALL ALRIGHT. Start resending last messege (trying #" + resendingCounter + ')\n';

		if (resendingCounter < 3)
		{
			mTcpSocket->close();
			QTimer::singleShot(100, [&]() {	connectToServer(tempBufferForLastMessege); });
		}
		else
		{
			mTcpSocket->close();
			timerForCheckSending->stop();
			resendingCounter = 0;
		}

		});
}


TcpSocketClass::~TcpSocketClass()
{
	if (mTcpSocket->isOpen()) {
		mTcpSocket->close();
	}
}

void TcpSocketClass::connectToServer(QByteArray messege)
{
	tempBufferForLastMessege = messege;

	if (!mTcpSocket->isOpen())
	{
		mTcpSocket->connectToHost(QHostAddress(host), port);
		qDebug() << "Connect to " + QHostAddress(host).toString();
	}

	timerForCheckSending->start(5000);
}


void TcpSocketClass::onConnected()
{
	// connectedState = true;
	qDebug() << "\nConnected to server\n";

	qDebug() << "TX << " << tempBufferForLastMessege.constData();

	mTcpSocket->write(tempBufferForLastMessege + "$" + QByteArray::number(tempBufferForLastMessege.length()));
}


void TcpSocketClass::onDisconnected()
{
	connectedState = false;
	qDebug() << "\nDisconnected from server.";
}



void TcpSocketClass::onReadyRead()
{
	QByteArray data = mTcpSocket->readAll();

	qDebug() << "RX << " << data.constData();

	if (data.constData() == QByteArray("OK"))
	{
		qDebug() << "ALL ALRIGHT";
		timerForCheckSending->stop();
		resendingCounter = 0;

		mTcpSocket->close();
	}

	if (data.constData() == QByteArray("RESEND"))
	{
		qDebug() << "CRC IS NOT CORRECT";
	}
}


void TcpSocketClass::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
	qDebug() << "\nSocket error:" << socketError << mTcpSocket->errorString();
	mTcpSocket->close();
}
