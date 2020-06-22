#pragma once
#include "FBaseSocket.h"
#include <QTcpSocket>
class FTcpClient : public FBaseSocket
{
public:
	FTcpClient();
	~FTcpClient();
	/*
	void sigSocketNewConnection(const QString &ip);
	void stgSocketDisConnection(const QString &ip);
	void sigSocketError(const QString &msg);
	//UI所用信号
	void sigSocketSend(const QString &msg);//
	void sigSocketReceive(const QString &msg);
	*/
protected:
	virtual bool start() override;
	virtual bool stop() override;
public slots:
	virtual void onSocketSend(const QString &msg) override;
protected slots :
	virtual void onSocketRecvNewConnection() override;
	virtual void onSocketConnected() override;
	virtual void onSocketDisconnected() override;
	virtual void onSocketError(QAbstractSocket::SocketError socketError) override;
	virtual void onSocketStateChanged(QAbstractSocket::SocketState socketState) override;
	virtual void onSocketReadReady() override;//数据读取
private:
	void setUpConnection();
	void disConnection();
private:
	QTcpSocket* _socketClient;
};

