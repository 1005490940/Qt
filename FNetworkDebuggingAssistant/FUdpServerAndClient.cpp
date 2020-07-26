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
	_socketUdp = new QUdpSocket(this);//����this ���� ��������ͬһ���߳�
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


QString FUdpServerAndClient::getSocketIpAddress(const QAbstractSocket *socket)
{
	if (socket == nullptr)
	{
		return QString();
	}
	return QString();
}

/*������Ϣ*/
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
	//�Լ��Ͽ�
	//�������Ͽ�
}

void FUdpServerAndClient::onSocketError(QAbstractSocket::SocketError socketError)
{

}

void FUdpServerAndClient::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	int a = 5;
}
/*UDP���յ���Ϣ*/
void FUdpServerAndClient::onSocketReadReady()
{
	QByteArray byteArray;
	QHostAddress senderIpAddress;//��ȡ���ͷ���IP��ַ
	quint16 sendPort = 0u;
	while (_socketUdp->hasPendingDatagrams())//���ڿɶ�����
	{
		byteArray.resize(_socketUdp->pendingDatagramSize());
		_socketUdp->readDatagram(byteArray.data(), byteArray.size(),&senderIpAddress,&sendPort);
	}
	QString &&clientIP = senderIpAddress.toString();
	QString &&timeInfo = (_configInfo->getShowRecvTimeFlag()? getCurrentDataTime() : QString());
	QString &&msg = QString(tr("[%1 %2]%3")).arg(clientIP).arg(timeInfo).arg(QString(byteArray));
	emit sigSocketReceive(msg);//֪ͨ����
}
