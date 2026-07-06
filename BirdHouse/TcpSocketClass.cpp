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
	if (mTcpSocket->isOpen()) 
	{
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

	if (!authBool)
		timerForCheckSending->start(8000);
}



void TcpSocketClass::onConnected()
{
	QDate curDate = QDate::currentDate();
	QTime curTime = QTime::currentTime();

	qDebug() << "\nConnected to server\n";

	qDebug() << "TX : " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString() << " << " << tempBufferForLastMessege.constData() << '\n';

	mTcpSocket->write(tempBufferForLastMessege + "%" + QByteArray::number(tempBufferForLastMessege.length()));
}



void TcpSocketClass::onDisconnected()
{
	connectedState = false;
	std::cout << "\n\nDisconnected from server.\n";
}



void TcpSocketClass::onReadyRead()
{
	data += mTcpSocket->readAll();

	if (data.size() > 1200000) // защита от переполнения буффера
	{
		qDebug() << "Buffer overflow, clearing data";
		data.clear();
		return;
	}

	bufferBool = true;

	if (bufferBool)
		QTimer::singleShot(200, [this]() {

		bufferBool = false;
		fullResultAccepted();

			});
}



void TcpSocketClass::fullResultAccepted()
{
	std::cout << "RX << " << data.constData();

	if (data.constData() == QByteArray("$&OK&$") && !authBool)
	{
		timerForCheckSending->stop();
		resendingCounter = 0;
		mTcpSocket->close();

		emit updateTaskAndOnInterface();
	}

	if (data.constData() == QByteArray("$&RESEND&$"))
	{
		qDebug() << "CRC IS NOT CORRECT";
	}

	QJsonParseError error;
	QJsonDocument jDoc = QJsonDocument::fromJson(data.constData(), &error);

	if (data.contains("status") && error.error == QJsonParseError::NoError)
	{
		if (jDoc.isNull()) 
		{
			qDebug() << "\nJSON parse error in TcpSocketClass::onReadyRead()";
			mTcpSocket->close();
		}
		else
		{
			QJsonObject rootArray = jDoc.object();

			if (rootArray["status"].toString() == "$&ACCESS&$")
			{
				emit accessAllowed(rootArray["userId"].toString(), rootArray["lastTask"].toString());
				authBool = false;
			}

			if (rootArray["status"].toString() == "$&ERROR&$")
			{
				emit statusBarMessege("Error when try to auth");
			}

			if (rootArray["status"].toString() == "$&NOTFOUNDUSER&$")
			{
				emit statusBarMessege("Not found this user in DB");
			}

			if (rootArray["status"].toString() == "$&REGISTER&$")
			{
				emit checkCodeInMail();
			}

			if (rootArray["status"].toString() == "$&REGISTERISDONE&$")
			{
				emit registerIsDoneSignal();
				emit statusBarMessege("Register is Done. Try Auth with new user");
			}

			if (rootArray["status"].toString() == "$&INCORRECTREGISTERCODE&$")
			{
				emit statusBarRegMessege("Try again send your actually code");
			}

			if (rootArray["status"].toString() == "$&USERREGISTEREARLIER&$")
			{
				emit statusBarRegMessege("User was register earlier. Try other user or auth");
			}

			if (rootArray["status"].toString() == "$&HISTORY&$")
			{
				emit transmitHistoryAnswer(jDoc);
			}

			timerForCheckSending->stop();
			resendingCounter = 0;
			mTcpSocket->close();
		}
	}

	data.clear();
}



void TcpSocketClass::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
	qDebug() << "\nSocket error:" << socketError << mTcpSocket->errorString();
	mTcpSocket->close();
}