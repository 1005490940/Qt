#include "FTcpClient.h"
#include <QDebug>
#include <QHostAddress>
FTcpClient::FTcpClient()
{
}


FTcpClient::~FTcpClient()
{
}

bool FTcpClient::start()
{
	_socketClient = new QTcpSocket(this);
	_socketClient->connectToHost(QHostAddress(getIpAddress()),getPort());//已在UI层做过滤 所以此处省略判断
	setUpConnection();
	return true;
}

bool FTcpClient::stop()
{
	return true;
}

QString FTcpClient::getSocketIpAddress(const QAbstractSocket *socket)
{
	return getIpAddress();
}

void FTcpClient::onSocketSend(const QString &msg)
{
	if (_socketClient)
	{
	    qint64 sendSize = _socketClient->write(msg.toUtf8());//
	}
}

void FTcpClient::onSocketRecvNewConnection()
{

}

void FTcpClient::onSocketConnected()
{
	auto port = _socketClient->localPort();
	setReceiverPort(port);
}

void FTcpClient::onSocketDisconnected()
{
	emit stgSocketDisConnection("");
}

void FTcpClient::onSocketError(QAbstractSocket::SocketError socketError)
{
	emit  sigSocketError("");
}

void FTcpClient::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{

}

void FTcpClient::onSocketReadReady()
{
	if (_socketClient)
	{
		QByteArray &&data = _socketClient->readAll();
		QString &&clientIP = getSocketIpAddress(_socketClient);
		QString &&timeInfo = (_configInfo->getShowRecvTimeFlag() ? getCurrentDataTime() : QString());
		QString &&msg = QString("【%1 %2】%3").arg(clientIP).arg(timeInfo).arg(QString(data));
		emit sigSocketReceive(msg);
	}
}

void FTcpClient::setUpConnection()
{
	connect(_socketClient, &QTcpSocket::connected, this, &FTcpClient::onSocketConnected);
	connect(_socketClient, &QTcpSocket::disconnected, this, &FTcpClient::onSocketDisconnected);
	connect(_socketClient, &QTcpSocket::stateChanged, this, &FTcpClient::onSocketStateChanged);
	connect(_socketClient, &QTcpSocket::readyRead, this, &FTcpClient::onSocketReadReady);
	connect(_socketClient, SIGNAL(error(QAbstractSocket::SocketError))
		, this, SLOT(onSocketError(QAbstractSocket::SocketError)));// 重载 所以使用QT4语法
}

void FTcpClient::disConnection()
{
	disconnect(_socketClient, &QTcpSocket::connected, this, &FTcpClient::onSocketConnected);
	disconnect(_socketClient, &QTcpSocket::disconnected, this, &FTcpClient::onSocketDisconnected);
	disconnect(_socketClient, &QTcpSocket::stateChanged, this, &FTcpClient::onSocketStateChanged);
	disconnect(_socketClient, &QTcpSocket::readyRead, this, &FTcpClient::onSocketReadReady);
	disconnect(_socketClient, SIGNAL(error(QAbstractSocket::SocketError))
		, this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}
