#include "FTcpServer.h"
FTcpServer::FTcpServer(QObject *parent)
	: FBaseSocket(parent)
	,_socketServer(nullptr)
{
	_socketServer = new QTcpServer(this);
	setUpConnection();
}

FTcpServer::~FTcpServer()
{
	//��ȷ�����������Դ
}


void FTcpServer::setUpConnection()
{
	connect(_socketServer, &QTcpServer::newConnection, this, &FTcpServer::onSocketRecvNewConnection);
}

QString FTcpServer::getClientIpAddressByClientSocket(QTcpSocket* client)
{
	for (auto iter = _ipAddress2ClientSocket.begin();iter != _ipAddress2ClientSocket.end();++iter)
	{
		if (iter.value() == client)
		{
			return iter.key();
		}
	}
	return QString("");//std::move(QString(""))
}

QTcpSocket* FTcpServer::getClientSocketByClientIpAddress(const QString &ip)
{
	for (auto iter = _ipAddress2ClientSocket.begin();iter != _ipAddress2ClientSocket.end();++iter)
	{
		if (iter.key() == ip)
		{
			return iter.value().data();
		}
	}
	return Q_NULLPTR;
}

void FTcpServer::onSocketRecvNewConnection()
{
	 
	QSharedPointer<QTcpSocket> socketClient = QSharedPointer<QTcpSocket>(_socketServer->nextPendingConnection());
	//��Ҫ�����ȡ�����ź�  �����ź�  �����ź�
	connect(socketClient.data(), &QTcpSocket::connected, this, &FTcpServer::onSocketConnected);
	connect(socketClient.data(), &QTcpSocket::disconnected, this, &FTcpServer::onSocketDisconnected);
	connect(socketClient.data(), &QTcpSocket::stateChanged, this, &FTcpServer::onSocketStateChanged);
	connect(socketClient.data(), &QTcpSocket::readyRead, this, &FTcpServer::onSocketReadReady);
	connect(socketClient.data(), SIGNAL(error(QAbstractSocket::SocketError socketError))
		, this, SLOT(onSocketError(QAbstractSocket::SocketError socketError)));
	//��ȡIP��ַ 
	QString strIP = tr("%1").arg(socketClient->peerAddress().toString().split("::ffff:")[1]);
	//����maP ֪ͨ������ˢ��
	_ipAddress2ClientSocket.insert(strIP, socketClient);
	emit sigSocketNewConnection(strIP);//������֪ͨ
}
/*�ͻ�������*/
void FTcpServer::onSocketConnected()
{
	
}
/*�ͻ��˶Ͽ�����*/
void FTcpServer::onSocketDisconnected()
{
	QSharedPointer<QTcpSocket> socketClient = QSharedPointer<QTcpSocket>( qobject_cast<QTcpSocket*>(sender()));
	if (socketClient == nullptr)
	{
		qWarning() << "client socket is null";
		return;
	}
	QString strIP = tr("%1").arg(socketClient->peerAddress().toString().split("::ffff:")[1]);
	_ipAddress2ClientSocket.remove(strIP);
//	emit stgSocketDisConnection(strIP);//IP�Ͽ�
}
/*�ͻ��˳���*/
void FTcpServer::onSocketError(QAbstractSocket::SocketError socketError)
{
	
}
/*�ͻ���״̬�����仯*/
void FTcpServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{

}
/*��ȡ�ͻ��˷��͹���������*/
void FTcpServer::onSocketReadReady()
{

}

bool FTcpServer::start()
{
	if (_ipAddress.isEmpty() || _port < 1 || _socketServer == nullptr)
	{
		return false;
	}
	QHostAddress addr(_ipAddress);
	_socketServer->listen(addr, _port);
	return true;
}

bool FTcpServer::stop()
{
	return true;
}

void FTcpServer::onSocketSend(const QString &msg)
{
	if (msg.isEmpty())
	{

	}
}

