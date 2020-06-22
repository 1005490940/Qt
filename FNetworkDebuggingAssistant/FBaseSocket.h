#pragma once

#include <QObject>
#include <QAbstractSocket>
class FBaseSocket : public QObject
{
	Q_OBJECT
public:
	enum  SendMode
	{
		TextMode,
		FileMdoe,
	};
	FBaseSocket(QObject *parent = Q_NULLPTR);
	~FBaseSocket();
	void setPort(unsigned int port);
	unsigned int getPort()const;
	void setIpAddress(const QString &ip);
	QString getIpAddress()const;
	void setReceiverIpAddress(const QString &ip);
	QString getReceiverIpAddress()const;
	unsigned int getReceiverPort()const;
	void setReceiverPort(unsigned int rPort);
	bool tryToStart();
	bool tryToStop();
protected:
	virtual bool start() = 0;
	virtual bool stop() = 0;

signals:
	void sigSocketNewConnection(const QString &ip);
	void stgSocketDisConnection(const QString &ip);
	void sigSocketError(const QString &msg);
//UI所用信号
	void sigSocketSend(const QString &msg);
	void sigSocketReceive(const QString &msg);
public slots:
	virtual void onSocketSend(const QString &msg) = 0;
protected slots :
	virtual void onSocketRecvNewConnection() =0;
	virtual void onSocketConnected() = 0;
	virtual void onSocketDisconnected() = 0;
	virtual void onSocketError(QAbstractSocket::SocketError socketError) = 0;
	virtual void onSocketStateChanged(QAbstractSocket::SocketState socketState) = 0;
	virtual void onSocketReadReady() = 0;//数据读取
protected:
	QString _ipAddress;
	unsigned int _port;
	QString _receiverIpAddress;
	unsigned int _receiverPort;
	SendMode _curMode;
};
