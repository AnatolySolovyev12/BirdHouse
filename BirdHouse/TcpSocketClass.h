#pragma once

#include <QObject>
#include <QTcpSocket>

class TcpSocketClass  : public QObject
{
	Q_OBJECT

public:
	TcpSocketClass(QObject *parent = nullptr);
	~TcpSocketClass();

public slots:
	void connectToServer(QString messege);


private slots:
	
	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
	QTcpSocket* mTcpSocket = nullptr;
	bool connectedState = false;
	QByteArray tempMessege;
	const QString host = "127.0.0.1";
	quint16 port = 49555;
};

