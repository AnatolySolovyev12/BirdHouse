#include "TcpSocketClass.h"

TcpSocketClass::TcpSocketClass(QObject* parent)
	: QObject(parent), mTcpSocket(new QTcpSocket), timerForCheckSending(new QTimer)
{
	connect(mTcpSocket, &QTcpSocket::connected, this, &TcpSocketClass::onConnected);
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &TcpSocketClass::onDisconnected);
	connect(mTcpSocket, &QTcpSocket::readyRead, this, &TcpSocketClass::onReadyRead);
	connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &TcpSocketClass::onErrorOccurred);

	connect(timerForCheckSending, &QTimer::timeout, [this]() {

		resendingCounter++;

		if (resendingCounter <= 3)
		{
			qDebug() << "NOT ALL ALRIGHT. Start resending last messege (trying #" + QString::number(resendingCounter) + ')';
			mTcpSocket->close();
			QTimer::singleShot(100, [&]() {	connectToServer(tempBufferForLastMessege); });
		}
		else
		{
			mTcpSocket->close();
			timerForCheckSending->stop();
			resendingCounter = 0;
			qDebug() << "Check your network connection to server";
		}

		});
}


TcpSocketClass::~TcpSocketClass()
{
	if (mTcpSocket->isOpen()) {
		mTcpSocket->close();
	}
}



void TcpSocketClass::setIpPort(QString ipSet, quint16 portSet)
{
	host = ipSet;
	port = portSet;
}



void TcpSocketClass::connectToServer(QByteArray messege)
{
	tempBufferForLastMessege = messege;

	if (!mTcpSocket->isOpen())
	{
		mTcpSocket->connectToHost(QHostAddress(host), port);
		qDebug() << '\n' << "Connect to " + QHostAddress(host).toString();
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
		timerForCheckSending->stop();
		resendingCounter = 0;
		mTcpSocket->close();

		emit updateTaskAndOnInterface();
	}

	if (data.constData() == QByteArray("RESEND"))
	{
		qDebug() << "CRC IS NOT CORRECT";
	}

	if (data.contains("status"))
	{
		QJsonDocument jDoc = QJsonDocument::fromJson(data.constData());

		if (jDoc.isNull()) {
			qDebug() << "JSON parse error in TcpSocketClass::onReadyRead()";
			mTcpSocket->close();
		}
		else
		{
			QJsonObject rootArray = jDoc.object();

			if (rootArray["status"].toString() == "ACCESS")
			{
				emit accessAllowed(rootArray["userId"].toString(), rootArray["lastTask"].toString());
			}
			if (rootArray["status"].toString() == "ERROR")
			{
				emit statusBarMessege("Error when try to auth");
			}
			if (rootArray["status"].toString() == "NOTFOUNDUSER")
			{
				emit statusBarMessege("Not found this user in DB");
			}
			if (rootArray["status"].toString() == "REGISTER")
			{
				emit checkCodeInMail();
			}


			timerForCheckSending->stop();
			resendingCounter = 0;
			authBool = false;
			mTcpSocket->close();
		}
	}
}



void TcpSocketClass::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
	qDebug() << "\nSocket error:" << socketError << mTcpSocket->errorString();
	mTcpSocket->close();
}