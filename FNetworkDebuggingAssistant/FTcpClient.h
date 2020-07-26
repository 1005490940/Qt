#pragma once
#include "FBaseSocket.h"
#include <QTcpSocket>
class FTcpClient : public FBaseSocket
{
public:
	FTcpClient();
	~FTcpClient();
protected:
	virtual bool start() override;
	virtual bool stop() override;
	virtual QString getSocketIpAddress(const QAbstractSocket *socket) override;
public slots:
	virtual void onSocketSend(const QString &msg) override;
protected slots :
	virtual void onSocketRecvNewConnection() override;
	virtual void onSocketConnected() override;
	virtual void onSocketDisconnected() override;
	virtual void onSocketError(QAbstractSocket::SocketError socketError) override;
	virtual void onSocketStateChanged(QAbstractSocket::SocketState socketState) override;
	virtual void onSocketReadReady() override;//Êý¾Ý¶ÁÈ¡
private:
	void setUpConnection();
	void disConnection();
private:
	QTcpSocket* _socketClient;
};

