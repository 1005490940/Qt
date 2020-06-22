#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QMap>
#include "FBaseSocket.h"
/*在次线程中执行*/
class FTcpServer : public FBaseSocket
{ 
	Q_OBJECT
public:
	FTcpServer(QObject *parent);
	~FTcpServer();
private:
	void setUpConnection();
	QString getClientIpAddressByClientSocket(QTcpSocket* client);
	QTcpSocket* getClientSocketByClientIpAddress(const QString &ip);
private slots:
    virtual void onSocketRecvNewConnection() override;
	virtual void onSocketConnected() override;
	virtual void onSocketDisconnected() override;
	virtual void onSocketError(QAbstractSocket::SocketError socketError) override;
	virtual void onSocketStateChanged(QAbstractSocket::SocketState socketState) override;
	virtual void onSocketReadReady() override;//数据读取
protected:
	virtual bool start() override;
	virtual bool stop() override;
public slots:
	virtual void onSocketSend(const QString &msg) override;
private:
	QTcpServer* _socketServer;//服务器
	QMap<QString, QSharedPointer<QTcpSocket>> _ipAddress2ClientSocket;
};
