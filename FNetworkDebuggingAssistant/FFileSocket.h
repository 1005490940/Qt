#pragma once

#include <QObject>
#include <QTcpSocket>
#include "FBaseSocket.h"
#include <QThread>
#include <QTcpServer>
class FFileSocket : public FBaseSocket
{
	Q_OBJECT

public:
	FFileSocket(QObject *parent);
	~FFileSocket();
private:
	QTcpSocket * _fileSendSocket;//���Ͷ˿�
	QTcpServer *_fileRecvSocket;//���ն�
};
