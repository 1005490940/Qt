#include "FBaseSocket.h"

FBaseSocket::FBaseSocket(QObject *parent )
	: QObject(parent)
	, _port(8080)
	,_receiverPort(8080)
	,_curMode(SendMode::TextMode)
{
}

FBaseSocket::~FBaseSocket()
{
	qDebug() << "===== ~FBaseSocket =======";
}

void FBaseSocket::setPort(unsigned int port)
{
	_port = port;
}

unsigned int FBaseSocket::getPort() const
{
	return _port;
}

void FBaseSocket::setIpAddress(const QString &ip)
{
	_ipAddress = ip;
}

QString FBaseSocket::getIpAddress() const
{
	return _ipAddress;
}

void FBaseSocket::setReceiverIpAddress(const QString &ip)
{
	_receiverIpAddress = ip;
}

QString FBaseSocket::getReceiverIpAddress() const
{
	return _receiverIpAddress;
}

unsigned int FBaseSocket::getReceiverPort() const
{
	return _receiverPort;
}

void FBaseSocket::setReceiverPort(unsigned int rPort)
{
	_receiverPort = rPort;
}

bool FBaseSocket::tryToStart()
{
	return start();
}

bool FBaseSocket::tryToStop()
{
	return stop();
}

