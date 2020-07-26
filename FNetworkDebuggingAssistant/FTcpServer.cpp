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
	//正确处理服务器资源
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
	//需要处理读取数据信号  下线信号  出错信号
	connect(socketClient.data(), &QTcpSocket::connected, this, &FTcpServer::onSocketConnected);
	connect(socketClient.data(), &QTcpSocket::disconnected, this, &FTcpServer::onSocketDisconnected);
	connect(socketClient.data(), &QTcpSocket::stateChanged, this, &FTcpServer::onSocketStateChanged);
	connect(socketClient.data(), &QTcpSocket::readyRead, this, &FTcpServer::onSocketReadReady);
	connect(socketClient.data(), SIGNAL(error(QAbstractSocket::SocketError))
		, this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	//获取IP地址 
	auto clientInfo = socketClient->peerAddress().toString().split("::ffff:");
	QString strIP = tr("%1").arg(clientInfo.at(0));
	//插入maP 通知主界面刷新
	_ipAddress2ClientSocket.insert(strIP, socketClient);
	emit sigNewConnection(strIP);//主界面通知
}
/*客户端连接*/
void FTcpServer::onSocketConnected()
{
	
}
/*客户端断开连接*/
void FTcpServer::onSocketDisconnected()
{
	QSharedPointer<QTcpSocket> socketClient = QSharedPointer<QTcpSocket>( qobject_cast<QTcpSocket*>(sender()));
	if (socketClient == nullptr)
	{
		qWarning() << "client socket is null";
		return;
	}
	QString strIP = tr("%1").arg(socketClient->peerAddress().toString().split("::ffff:")[0]);
	_ipAddress2ClientSocket.remove(strIP);
	emit stgSocketDisConnection(strIP);//IP断开
}
/*客户端出错*/
void FTcpServer::onSocketError(QAbstractSocket::SocketError socketError)
{
	
}
/*客户端状态发生变化*/
void FTcpServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{

}
/*读取客户端发送过来的数据*/
void FTcpServer::onSocketReadReady()
{
	auto tcpClient = dynamic_cast<QTcpSocket*>(sender());
	if (tcpClient)
	{
		QByteArray &&data = tcpClient->readAll();
		QString &&clientIP = getSocketIpAddress(tcpClient);
		QString &&timeInfo = (_configInfo->getShowRecvTimeFlag()? getCurrentDataTime() : QString());
		QString &&msg = QString("【%1 %2】%3").arg(clientIP).arg(timeInfo).arg(QString(data));
		emit sigSocketReceive(msg);
	}
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

QString FTcpServer::getSocketIpAddress(const QAbstractSocket *socket)
{
	if (socket == nullptr)
	{
		return QString();
	}
	return socket->peerAddress().toString();
}

void FTcpServer::onSocketSend(const QString &msg)
{
	auto recvClientIp = getReceiverIpAddress();
	if (recvClientIp.isEmpty())
	{
		for ( auto iter = _ipAddress2ClientSocket.begin();iter!= _ipAddress2ClientSocket.end();++iter)
		{
			auto tcpClient = iter.value();
			if (tcpClient)
			{
				tcpClient->write(msg.toUtf8());
			}
		}
	}
	else
	{
		auto recvClient = getClientSocketByClientIpAddress(recvClientIp);
		if (recvClient)
		{
			recvClient->write(msg.toUtf8());
		}
	}
}

