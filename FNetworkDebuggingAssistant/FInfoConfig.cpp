#include "FInfoConfig.h"

FInfoConfig::FInfoConfig()
	:_ipAddress("127.0.0.1")
	, _port(8080)
	, _isRecvSaveToFile(false)
	, _isShowRecvTime(false)
	, _isShow0x(false)
	, _isPauseRecv(false)
	, _isFileDataSource(false)
	, _isAutoRevcAdd(false)
	, _isSendClear(false)
	, _isSendByOx(false)
	, _isSendLoop(false)
	, _sendPauseTime(1000)
	, _ptcType(ProtocolType::UDP)
	,_saveFilePath(QString())
{
}

FInfoConfig::~FInfoConfig()
{
}

void FInfoConfig::setProtocolType(int type)
{
	auto ptcType = static_cast<FInfoConfig::ProtocolType>(type);
	setType(ptcType);
}

int FInfoConfig::getProtocolType() const
{
	return static_cast<int>(getType());
}

void FInfoConfig::setType(ProtocolType type)
{
	_ptcType = type;
}

FInfoConfig::ProtocolType FInfoConfig::getType() const
{
	return _ptcType;
}

void FInfoConfig::setIpAddress(const QString &ip)
{
	_ipAddress = ip;//std::move
}


QString  FInfoConfig::getIpAddress() const
{
	return _ipAddress;
}

void FInfoConfig::setPort(unsigned int port)
{
	_port = port;
}

unsigned int FInfoConfig::getPort() const
{
	return _port;
}

void FInfoConfig::setSaveToFileFlag(bool val)
{
	_isRecvSaveToFile = val;
}

bool FInfoConfig::getSaveToFileFlag() const
{
	return _isRecvSaveToFile;
}

void FInfoConfig::setSaveFilePath(const QString &filePath)
{
	_saveFilePath = filePath;
}

const QString & FInfoConfig::getSaveFilePath() const
{
	return _saveFilePath;
}

void FInfoConfig::setShowRecvTimeFlag(bool val)
{
	_isShowRecvTime = val;
}

bool FInfoConfig::getShowRecvTimeFlag() const
{
	return _isShowRecvTime;
}

void FInfoConfig::setShow0xFlag(bool val)
{
	_isShow0x = val;
}

bool FInfoConfig::getShow0xFlag() const
{
	return _isShow0x;
}

void FInfoConfig::setPauseRecvFlag(bool val)
{
	_isPauseRecv = val;
}

bool FInfoConfig::getPauseRecvFlag() const
{
	return _isPauseRecv;
}

void FInfoConfig::setFileDataSourceFlag(bool val)
{
	_isFileDataSource = val;
}

bool FInfoConfig::getFileDataSourceFlag() const
{
	return _isFileDataSource;
}

void FInfoConfig::setAutoSendAddFlag(bool val)
{
	_isAutoRevcAdd = val;
}

bool FInfoConfig::getAutoSendAddFlag() const
{
	return _isAutoRevcAdd;
}

void FInfoConfig::setSendClearFlag(bool val)
{
	_isSendClear = val;
}

bool FInfoConfig::getSendClearFlag() const
{
	return _isSendClear;
}

void FInfoConfig::setSendBy0xFlag(bool val)
{
	_isSendByOx = val;
}

bool FInfoConfig::getSendBy0xFlag() const
{
	return _isSendByOx;
}

void FInfoConfig::setSendLoopFlag(bool val)
{
	_isSendLoop = val;
}

bool FInfoConfig::getSendLoopFlag() const
{
	return _isSendLoop;
}

void FInfoConfig::setSendPauseTime(unsigned int val)
{
	_sendPauseTime = val;
}

unsigned int FInfoConfig::getSendPauseTime() const
{
	return _sendPauseTime;
}
