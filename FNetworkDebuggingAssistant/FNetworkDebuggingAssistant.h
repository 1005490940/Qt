#pragma once

#include <QtWidgets/QWidget>
#include "ui_FNetworkDebuggingAssistant.h"
#include <QSharedPointer>
#include <QMenu>
#include <QAction>
#include <QThread>//socket线程
#include <QSystemTrayIcon>
#include <QValidator>
#include "FInfoConfig.h"
#include "FTcpServer.h"
class FNetworkDebuggingAssistant : public QWidget
{
    Q_OBJECT
public:
    FNetworkDebuggingAssistant(QWidget *parent = Q_NULLPTR);
	~FNetworkDebuggingAssistant();
	void setXmlPath(const QString &path);
	QString getXmlPath()const;
	bool  tryToLoadConfigInfoFromXml(const QString &xmlPth);
	bool tryToSaveConfigInfoToXml(const QString &xmlPth);
protected:
	bool loadConfigInfoFromXml(const QString &xmlPth);
	bool saveConfigInfoToXml(const QString &xmlPth);
	void initNetInfo();
private:
	void setUpConnection();
	void initUi();
	void setMidWidgetGroupVisible(bool visible);
	void initTrayMenu();
protected slots: //socket槽函数  //在主界面过滤
    void onReceiveMessage(const QString &msg);
signals:
    void toSendMessage(const QString &msg); 
protected slots ://UI 槽函数 
	void onProtocolTypeIndexChanged(int index);
    void onBuildSocket(bool);
	void onSaveRecvDataAsFile(bool);
	void onClearAllRecvData(bool);
	void onLoadingFile(bool);
	void onClearInputArea(bool);
	void onResetCount(bool);
	void onSendMessageToSocket(bool);
	void onSystemQuit(bool);
private:
	void outPutWarnningIntof(const std::wstring &msg);
	void setUpConfigInfoByUi();
	void setUpUiByConfigInfo();
	void updateButtonText(bool isBuild);
	void updateUi();
	void destoryResources();
	void updateSendSize(unsigned long long sendSize);
	void updateRecvSize(unsigned long long recvSize);
private:
	bool connectSocket();
	bool disconnectSocket();
	bool createUDPSocket(const FInfoConfig *configInfo);
	bool createTCPClientSocket(const FInfoConfig *configInfo);
	bool createTCPServerSocket(const FInfoConfig *configInfo);
private:
	Ui::FNetworkDebuggingAssistantClass ui;
	QSharedPointer<FInfoConfig> _configInfo;
	QString _xmlPath;
	QSharedPointer<QSystemTrayIcon> _sysTray;//系统托盘菜单
	QSharedPointer<QMenu> _sysMenu;
	QSharedPointer<QAction> _actionQuit;
	QSharedPointer<QAction> _actionConnect;
	FBaseSocket* _socketBase;//socket基类

	QThread* _threadSocket;
	bool _isBulidSocket;
	unsigned long long _sizeRecv;
	unsigned long long _sizeSend;
};
