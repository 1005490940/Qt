#include "FUdpServerAndClient.h"
#include "FQStringUtil.h"
#include <QDebug>
FUdpServerAndClient::FUdpServerAndClient(QObject *parent)
	: FBaseSocket(parent)
	,_socketUdp(nullptr)
{

}

FUdpServerAndClient::~FUdpServerAndClient()
{
    if (_socketUdp)
    {
		disconnect(_socketUdp, &QUdpSocket::stateChanged, this, &FUdpServerAndClient::onSocketStateChanged);
		disconnect(_socketUdp, &QUdpSocket::readyRead, this, &FUdpServerAndClient::onSocketReadReady);//
    }
	qDebug() << "===== ~FUdpServerAndClient =======";
}

bool FUdpServerAndClient::start()
{	
	_socketUdp = new QUdpSocket(this);//不给this 崩溃 创建不在同一个线程
 	connect(_socketUdp, &QUdpSocket::stateChanged, this, &FUdpServerAndClient::onSocketStateChanged);
	connect(_socketUdp, &QUdpSocket::readyRead, this, &FUdpServerAndClient::onSocketReadReady);
	if (_socketUdp->bind(QHostAddress(_ipAddress),_port))
	{
		qDebug() << "bind success";
	}
	else
	{
		qDebug() << "bind failed";
	}

	return true;
}

bool FUdpServerAndClient::stop()
{
	if (_socketUdp)
	{
		disconnect(_socketUdp, &QUdpSocket::stateChanged, this, &FUdpServerAndClient::onSocketStateChanged);
		disconnect(_socketUdp, &QUdpSocket::readyRead, this, &FUdpServerAndClient::onSocketReadReady);//
		_socketUdp->deleteLater();
	}
	return true;
}


/*发送信息*/
void FUdpServerAndClient::onSocketSend(const QString &msg)
{
	qint64 len = _socketUdp->writeDatagram(msg.toUtf8(), QHostAddress(_receiverIpAddress), _receiverPort);
}

void FUdpServerAndClient::onSocketRecvNewConnection()
{

}

void FUdpServerAndClient::onSocketConnected()
{
	qDebug() << "connect sever success";
}

void FUdpServerAndClient::onSocketDisconnected()
{
	//自己断开
	//服务器断开
}

void FUdpServerAndClient::onSocketError(QAbstractSocket::SocketError socketError)
{

}

void FUdpServerAndClient::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	int a = 5;
}
/*UDP接收到信息*/
void FUdpServerAndClient::onSocketReadReady()
{
	QByteArray byteArray;
	while (_socketUdp->hasPendingDatagrams())//存在可读数据
	{
		byteArray.resize(_socketUdp->pendingDatagramSize());
		_socketUdp->readDatagram(byteArray.data(), byteArray.size());
	}
	emit sigSocketReceive(QString(byteArray));//通知界面
}
