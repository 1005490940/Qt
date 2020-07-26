#pragma once

#include <QObject>
#include <QString>
class FInfoConfig
{
		enum ProtocolType
	{
		UDP,
		TCPCLIENT,
		TCPSERVER,
	};
public:
	FInfoConfig();
	~FInfoConfig();
	void setProtocolType(int type);
	int getProtocolType()const;
	void setIpAddress(const QString &ip);
	QString getIpAddress()const;
	void setPort(unsigned int port);
	unsigned int getPort()const;
	void setSaveToFileFlag(bool val);
	bool getSaveToFileFlag()const;
	void setSaveFilePath(const QString &filePath);
	const QString &getSaveFilePath()const;
	void setShowRecvTimeFlag(bool val);
	bool getShowRecvTimeFlag()const;
	void setShow0xFlag(bool val);
	bool getShow0xFlag()const;
	void setPauseRecvFlag(bool val);
	bool getPauseRecvFlag()const;
	void setFileDataSourceFlag(bool val);
	bool getFileDataSourceFlag()const;
	void setAutoSendAddFlag(bool val);
	bool getAutoSendAddFlag()const;
	void setSendClearFlag(bool val);
	bool getSendClearFlag()const;
	void setSendBy0xFlag(bool val);
	bool getSendBy0xFlag()const;
	void setSendLoopFlag(bool val);
	bool getSendLoopFlag()const;
	void setSendPauseTime(unsigned int val);
	unsigned int getSendPauseTime()const;
	void setType(ProtocolType type);
	ProtocolType getType()const;
private:
	QString _ipAddress;
	unsigned int _port;
	bool _isRecvSaveToFile;
	bool _isShowRecvTime;
	bool _isShow0x;
	bool _isPauseRecv;
	//∑¢ÀÕ«¯”Ú…Ë÷√
	bool _isFileDataSource;
	bool _isAutoRevcAdd;
	bool _isSendClear;
	bool _isSendByOx;
	bool _isSendLoop;
	unsigned int _sendPauseTime;
	ProtocolType _ptcType;
    QString _saveFilePath;
};
