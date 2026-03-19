#pragma once

#include <QObject>
#include <QTcpSocket>
#include <qtimer.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class TcpSocketClass : public QObject
{
	Q_OBJECT

public:
	TcpSocketClass(QObject* parent = nullptr);
	~TcpSocketClass();

	void setIpPort(QString ipSet, quint16 portSet);




signals:
	void accessAllowed(QString iduser, QString lastTask);
	void statusBarMessege(QString text);





public slots:
	void connectToServer(QByteArray messege);


private slots:

	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onErrorOccurred(QAbstractSocket::SocketError socketError);


private:
	QTcpSocket* mTcpSocket = nullptr;
	bool connectedState = false;
	QString host = "127.0.0.1";
	quint16 port = 49555;
	QTimer* timerForCheckSending;
	int resendingCounter = 0;
	QByteArray tempBufferForLastMessege;
	bool authBool = true;
};

