#pragma once

#include <QObject>
#include <QTcpSocket>
#include <qtimer.h>

class TcpSocketClass  : public QObject
{
	Q_OBJECT

public:
	TcpSocketClass(QObject *parent = nullptr);
	~TcpSocketClass();

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
	const QString host = "127.0.0.1";
	quint16 port = 49555;
	QTimer* timerForCheckSending;
	int resendingCounter = 0;
	QByteArray tempBufferForLastMessege;
};

