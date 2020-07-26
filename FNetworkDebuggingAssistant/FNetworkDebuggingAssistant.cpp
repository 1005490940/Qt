#include "FNetworkDebuggingAssistant.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QHostInfo>
#include <QComboBox>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QSignalBlocker>
#include "FQStringUtil.h"
#include "FUdpServerAndClient.h"
#include "FTcpClient.h"
#include "FTcpServer.h"
FNetworkDebuggingAssistant::FNetworkDebuggingAssistant(QWidget *parent)
	: QWidget(parent)
	, _isBulidSocket(false)
	, _sizeSend(0)
	, _sizeRecv(0)
	,_actionCurrentSkin(nullptr)
{
	ui.setupUi(this);
	initUi();
	setUpConnection();
	initNetInfo();
	_configInfo = QSharedPointer<FInfoConfig>(new FInfoConfig);
	ui.lineEditPort->setText(QString("%1").arg(_configInfo->getPort()));
	initSkin();
}

FNetworkDebuggingAssistant::~FNetworkDebuggingAssistant()
{
	destoryResources();
}

void FNetworkDebuggingAssistant::setXmlPath(const QString &path)
{
	_xmlPath = path;
}

QString FNetworkDebuggingAssistant::getXmlPath() const
{
	return _xmlPath;
}

bool FNetworkDebuggingAssistant::tryToLoadConfigInfoFromXml(const QString &xmlPth)
{
	return loadConfigInfoFromXml(xmlPth);
}

bool FNetworkDebuggingAssistant::tryToSaveConfigInfoToXml(const QString &xmlPth)
{
	return saveConfigInfoToXml(xmlPth);
}

bool FNetworkDebuggingAssistant::loadConfigInfoFromXml(const QString &xmlPth)
{
	if (xmlPth.isEmpty())
	{
		qCritical("xml path is not exit");
		return false;
	}
	return true;
}

bool FNetworkDebuggingAssistant::saveConfigInfoToXml(const QString &xmlPth)
{
	return true;
}
/*提取出有效的IP地址*/
void FNetworkDebuggingAssistant::initNetInfo()
{
	QString hostNme = QHostInfo::localHostName();//为中文 无法解析
	QHostInfo hostInfo = QHostInfo::fromName(hostNme);
	auto addList = hostInfo.addresses();
	if (!addList.isEmpty())
	{
		ui.lineEditIpAddress->setText(addList[addList.size() - 1].toString());
	}
}

void FNetworkDebuggingAssistant::saveReceiveArea()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"",
		tr("All Files (*.*) Text File(*.txt) Data File(*."));
	if (fileName.isEmpty())
	{
		return;
	}
	QFile saveFile(fileName);
	saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (saveFile.isOpen())
	{
		QString &&receiveText = ui.textEditOutPut->toPlainText();
		QTextStream outStream(&saveFile);
		outStream << receiveText;
		saveFile.close();
	}
}

void FNetworkDebuggingAssistant::setUpConnection()
{
	connect(ui.pushButtonConnect, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onBuildSocket);
	connect(ui.comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onProtocolTypeIndexChanged(int)));
	connect(ui.checkBoxFile, &QCheckBox::clicked, this, &FNetworkDebuggingAssistant::onReceiveToFile);
	connect(ui.checkBoxTime, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setShowRecvTimeFlag(ui.checkBoxTime->isChecked());
	});
	connect(ui.checkBox0x, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setShow0xFlag(ui.checkBox0x->isChecked());
	});
	connect(ui.checkBoxPause, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setPauseRecvFlag(ui.checkBoxPause->isChecked());
	});
	connect(ui.checkBoxFileDataSource, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setFileDataSourceFlag(ui.checkBoxFileDataSource->isChecked());
	});
	connect(ui.checkBoxAdd, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setAutoSendAddFlag(ui.checkBoxAdd->isChecked());
	});
	connect(ui.checkBoxClear, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setSendClearFlag(ui.checkBoxClear->isChecked());
	});
	connect(ui.checkBoxSend0x, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setSendBy0xFlag(ui.checkBoxSend0x->isChecked());
	});
	connect(ui.checkBoxLoop, &QCheckBox::clicked, this, [=](bool)
	{
		_configInfo->setSendLoopFlag(ui.checkBoxLoop->isChecked());
	});
	connect(ui.pushButtonSave, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onSaveRecvDataAsFile);
	connect(ui.pushButtonClearJOutPut, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onClearAllRecvData);
	connect(ui.pushButtonLoadFile, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onLoadingFile);
	connect(ui.pushButtonClearInput, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onClearInputArea);
	connect(ui.pushButtonResetCount, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onResetCount);
	connect(ui.pushButtonSend, &QPushButton::clicked, this, &FNetworkDebuggingAssistant::onSendMessageToSocket);
}

void FNetworkDebuggingAssistant::initUi()
{
	ui.widgetServer->setVisible(false);
	ui.widgetClient->setVisible(false);
	ui.labelRevSize->setScaledContents(true);
	ui.labelSendSize->setScaledContents(true);
	initTrayMenu();
	//正则表达式 限制 IP地址的输入
	QRegExp regExpIpAddress("^([1]?/d/d?|2[0-4]/d|25[0-5])/.([1]?/d/d?|2[0-4]/d|25[0-5])/.([1]?/d/d?|2[0-4]/d|25[0-5])/.([1]?/d/d?|2[0-4]/d|25[0-5])$");
	// QRegExp rxp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	ui.lineEditIpAddress->setValidator(new QRegExpValidator(regExpIpAddress, this));
	//限制端口的输入
	QRegExp regExpPort("^([0-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])$");
	ui.lineEditPort->setValidator(new QRegExpValidator(regExpPort, this));
}

void FNetworkDebuggingAssistant::setMidWidgetGroupVisible(bool visible)
{
	ui.widgetServer->setVisible(visible);
	ui.widgetClient->setVisible(!visible);
}


void FNetworkDebuggingAssistant::initTrayMenu()
{
	_sysTray = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon(this));
	_sysMenu = QSharedPointer<QMenu>(new QMenu);
	_actionQuit = QSharedPointer<QAction>(new QAction(nullptr));
	_actionQuit->setText(W2Q(L"退出"));
	connect(_actionQuit.data(), &QAction::triggered, this, &FNetworkDebuggingAssistant::onSystemQuit);
	_actionConnect = QSharedPointer<QAction>(new QAction(nullptr));
	_actionConnect->setText(W2Q(L"连接"));
	_sysMenu->addAction(_actionConnect.data());
	_sysMenu->addAction(_actionQuit.data());
	_sysTray->setContextMenu(_sysMenu.data());
	_sysTray->setIcon(QIcon("data/icon/system.jpg"));
	_sysTray->setToolTip(W2Q(L"系统菜单"));
	_sysTray->show();
}

void FNetworkDebuggingAssistant::onProtocolTypeIndexChanged(int index)
{
	if (index < 0)
	{
		qWarning() << "current protocol type is not valid";
		return;
	}
	_configInfo->setProtocolType(index);

}

void FNetworkDebuggingAssistant::onBuildSocket(bool)
{
	if (_isBulidSocket)
	{
		disconnectSocket();
	}
	else
	{
		connectSocket();
	}
	_isBulidSocket = !_isBulidSocket;
	updateUi();
	
}

void FNetworkDebuggingAssistant::onSaveRecvDataAsFile(bool)
{
	saveReceiveArea();
}

void FNetworkDebuggingAssistant::onClearAllRecvData(bool)
{
	ui.textEditOutPut->clear();
}

void FNetworkDebuggingAssistant::onLoadingFile(bool)
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/home",
		tr("AllFiles (*.*)"));
	if (fileName.isEmpty())
	{
		return;
	}
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QByteArray &&byteArray = file.readAll();
	ui.textEdit->setText(QString(byteArray));
	file.close();
}

void FNetworkDebuggingAssistant::onClearInputArea(bool)
{
	ui.textEdit->clear();
}

void FNetworkDebuggingAssistant::onResetCount(bool)
{
	updateRecvSize(0);
	updateSendSize(0);
}

void FNetworkDebuggingAssistant::onSendMessageToSocket(bool)
{
	if (_socketBase == nullptr || !_isBulidSocket)
	{
		ui.labelInfo->setText(W2Q(L"发送失败"));
		return;
	}
	auto recvIp = ui.lineEditTargetIp->text();
	auto recvPort = ui.lineEditTargetPort->text().toUInt();
	if (_configInfo->getType() == 0)
	{
		if (recvIp.isEmpty() || recvPort < 1)
		{
			outPutWarnningIntof(L"请输入接收方的IP地址或者端口号");
			return;
		}

	}
	_socketBase->setReceiverIpAddress(recvIp);
	_socketBase->setReceiverPort(recvPort);
	QString msg = ui.textEdit->toPlainText();
	_sizeSend += msg.length();
	updateSendSize(_sizeSend);
	emit toSendMessage(msg);
}

void FNetworkDebuggingAssistant::onReceiveMessage(const QString &msg)
{
	ui.textEditOutPut->append(msg);
	_sizeRecv += msg.length();
	updateRecvSize(_sizeRecv);
}

void FNetworkDebuggingAssistant::onNewConnection(const QString &ipAddress)
{
	ui.comboBoxConnectionObject->addItem(ipAddress);
}

void FNetworkDebuggingAssistant::onDisConnection(const QString &ipAddress)
{
	auto itemCount = ui.comboBoxConnectionObject->count();
	for (int i = 0;i<itemCount;++i)
	{
		auto itemText = ui.comboBoxConnectionObject->itemText(i);
		if (itemText == ipAddress)
		{
			ui.comboBoxConnectionObject->removeItem(i);
			return;
		}
	}
}

/*资源释放*/
void FNetworkDebuggingAssistant::onSystemQuit(bool)
{
	destoryResources();
	qApp->quit();
}

void FNetworkDebuggingAssistant::onReceiveToFile(bool)
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"",
		tr("All Files (*.*) Text File(*.txt) Data File(*."));
	if (fileName.isEmpty())
	{
		QSignalBlocker blocker(ui.checkBoxFile);
		ui.checkBoxFile->setChecked(false);//上面阻塞，不然会再次出发此信号 会引起递归
		return;
	}
	_configInfo->setSaveFilePath(fileName);
}

void FNetworkDebuggingAssistant::onSkinChange(bool)
{
	QAction * skinAction = dynamic_cast<QAction*>(sender());
	if (skinAction == nullptr)
	{
		return;
	}
	if (_actionCurrentSkin == skinAction)
	{
		return;//已经为当前皮肤 不用切换
	}
	
	QString &&skinFilePath = getSkinFilePathBySkinAction(skinAction);
	this->setStyleSheet(skinFilePath);
	_actionCurrentSkin = skinAction;
}

QString FNetworkDebuggingAssistant::getSkinFilePathBySkinAction(QAction *skinAction)
{
	auto iter = _skinAction2SkinFileName.find(skinAction);
	if (iter == _skinAction2SkinFileName.end())
	{
		//没有找到，继续使用当前皮肤
		// _skinAction2SkinFileName.value(_acitonSkinBlack.data());
		return _skinAction2SkinFileName.value(_actionCurrentSkin);//此处可能为空
	}
	return iter.value();
}

void FNetworkDebuggingAssistant::outPutWarnningIntof(const std::wstring &msg)
{
	auto textColor = ui.textEditOutPut->textColor();
	ui.textEditOutPut->setTextColor(QColor(255, 0, 0));
	ui.textEditOutPut->append(W2Q(msg));
	ui.textEditOutPut->setTextColor(textColor);
}

void FNetworkDebuggingAssistant::setUpConfigInfoByUi()
{
	if (_configInfo == nullptr)
	{
		qWarning() << "config object is null";
		return;
	}
	_configInfo->setProtocolType(ui.comboBoxType->currentIndex());
	_configInfo->setIpAddress(ui.lineEditIpAddress->text());
	_configInfo->setPort(ui.lineEditPort->text().toInt());
	//接收区设置
	_configInfo->setSaveToFileFlag(ui.checkBoxFile->isChecked());
	_configInfo->setShowRecvTimeFlag(ui.checkBoxTime->isChecked());
	_configInfo->setShow0xFlag(ui.checkBox0x->isChecked());
	_configInfo->setPauseRecvFlag(ui.checkBoxPause->isChecked());
	//发送区设置
	_configInfo->setFileDataSourceFlag(ui.checkBoxFileDataSource->isChecked());
	_configInfo->setAutoSendAddFlag(ui.checkBoxAdd->isChecked());
	_configInfo->setSendClearFlag(ui.checkBoxClear->isChecked());
	_configInfo->setSendBy0xFlag(ui.checkBoxSend0x->isChecked());
	_configInfo->setSendLoopFlag(ui.checkBoxLoop->isChecked());
}

void FNetworkDebuggingAssistant::setUpUiByConfigInfo()
{
	if (_configInfo == nullptr)
	{
		return;
	}
	ui.comboBoxType->setCurrentIndex(_configInfo->getProtocolType());
	ui.lineEditIpAddress->setText(_configInfo->getIpAddress());
	ui.lineEditPort->setText(QString("%1").arg(_configInfo->getPort()));
	//接收区
	ui.checkBoxFile->setChecked(_configInfo->getSaveToFileFlag());
	ui.checkBoxTime->setChecked(_configInfo->getShowRecvTimeFlag());
	ui.checkBox0x->setChecked(_configInfo->getShow0xFlag());
	ui.checkBoxPause->setChecked(_configInfo->getPauseRecvFlag());
	//发送区设置
	ui.checkBoxFileDataSource->setChecked(_configInfo->getFileDataSourceFlag());
	ui.checkBoxAdd->setChecked(_configInfo->getAutoSendAddFlag());
	ui.checkBoxClear->setChecked(_configInfo->getSendClearFlag());
	ui.checkBoxSend0x->setChecked(_configInfo->getSendBy0xFlag());
	ui.checkBoxLoop->setChecked(_configInfo->getSendLoopFlag());
}

void FNetworkDebuggingAssistant::updateButtonText(bool isBuild)
{
	if (isBuild)
	{
		ui.pushButtonConnect->setText(W2Q(L"断开"));
	}
	else
	{
		ui.pushButtonConnect->setText(W2Q(L"连接"));
	}
}

void FNetworkDebuggingAssistant::updateUi()
{
	if (!_isBulidSocket)
	{
		ui.labelInfo->setText(W2Q(L"连接失败"));
	}
	updateButtonText(_isBulidSocket);
	ui.comboBoxType->setEnabled(!_isBulidSocket);
	ui.lineEditIpAddress->setEnabled(!_isBulidSocket);
	ui.lineEditPort->setEnabled(!_isBulidSocket);
	if (_isBulidSocket)
	{
		switch (ui.comboBoxType->currentIndex())
		{
		case 0://UDP
		{
			ui.widgetClient->setVisible(true);
			ui.widgetServer->setVisible(false);
			ui.lineEditTargetIp->setText(_configInfo->getIpAddress());
			ui.lineEditTargetPort->setText(QString("%1").arg(_configInfo->getPort()));
		}
		break;
		case 1://Tcp Client
		{
			ui.widgetClient->setVisible(true);
			ui.widgetClient->setEnabled(false);
			ui.widgetServer->setVisible(false);
			ui.label_7->setText(W2Q(L"本地主机："));
			ui.label_9->setText(W2Q(L"本地端口："));
			ui.lineEditTargetIp->setText(_configInfo->getIpAddress());
			ui.lineEditTargetPort->setText(QString("%1").arg(_socketBase->getReceiverPort()));
		}
		break;
		case 2://TCP  Server
		{
			ui.widgetClient->setVisible(false);
			ui.widgetServer->setVisible(true);
		}
		break;
		default:
		{
			ui.widgetClient->setVisible(false);
			ui.widgetServer->setVisible(false);
		}
		break;
		}
	}
	else
	{
		ui.widgetClient->setVisible(false);
		ui.widgetServer->setVisible(false);
	}
}

void FNetworkDebuggingAssistant::destoryResources()
{
	if (_socketBase)
	{
		_socketBase->deleteLater();//会在一次事件循环完成只会再删除内存  也会调用 delete来释放内存
		disconnect(_socketBase, &FBaseSocket::sigSocketReceive, this, &FNetworkDebuggingAssistant::onReceiveMessage);
		disconnect(this, &FNetworkDebuggingAssistant::toSendMessage, _socketBase, &FBaseSocket::onSocketSend);
		disconnect(_socketBase, &FBaseSocket::sigNewConnection, this, &FNetworkDebuggingAssistant::onNewConnection);
		disconnect(_socketBase, &FBaseSocket::stgSocketDisConnection, this, &FNetworkDebuggingAssistant::onNewConnection);
		
		_socketBase = nullptr;
	}
	if (_threadSocket)
	{
		
		_threadSocket->quit();
		_threadSocket->wait();
		//_threadSocket->exit();
		_threadSocket->deleteLater();
		_threadSocket = nullptr;
	}
}

void FNetworkDebuggingAssistant::updateSendSize(unsigned long long sendSize)
{
	ui.labelSendSize->setText(QString("%1").arg(sendSize));
}

void FNetworkDebuggingAssistant::updateRecvSize(unsigned long long recvSize)
{
	ui.labelRevSize->setText(QString("%1").arg(recvSize));
}

bool FNetworkDebuggingAssistant::connectSocket()
{
	setUpConfigInfoByUi();
	auto type = _configInfo->getProtocolType();
	switch (type)
	{
	case  0:
		return  createUDPSocket(_configInfo.data());
		break;
	case 1:
		return  createTCPClientSocket(_configInfo.data());
		break;
	case 2:
		return  createTCPServerSocket(_configInfo.data());
		break;
	default:
		return false;
		break;
	}
	ui.comboBoxConnectionObject->addItem("All Connections");
}

bool FNetworkDebuggingAssistant::createUDPSocket(const FInfoConfig *configInfo)
{
	if (configInfo == nullptr)
	{
		qWarning() << "config info is empty";
		return false;
	}
	if (_socketBase)
	{
		destoryResources();
	}
	_socketBase = new FUdpServerAndClient;
	_socketBase->setIpAddress(configInfo->getIpAddress());
	_socketBase->setPort(configInfo->getPort());
	_socketBase->setConfigInfoManager(configInfo);
	_socketBase->tryToStart();//之前放在  _threadSocket->start(); 之后 出错
	_threadSocket = new QThread; // 
	_socketBase->moveToThread(_threadSocket);//
	_threadSocket->start();
	connect(_socketBase,&FBaseSocket::sigSocketReceive,this,&FNetworkDebuggingAssistant::onReceiveMessage);
	connect(_socketBase, &FBaseSocket::sigNewConnection, this, &FNetworkDebuggingAssistant::onNewConnection);
	connect(this, &FNetworkDebuggingAssistant::toSendMessage, _socketBase, &FBaseSocket::onSocketSend);
	return true;
}

bool FNetworkDebuggingAssistant::disconnectSocket()
{
	if (_socketBase == nullptr)
	{
		return false;
	}
	destoryResources();
	ui.comboBoxConnectionObject->clear();
	return true;
}

bool FNetworkDebuggingAssistant::createTCPClientSocket(const FInfoConfig *configInfo)
{ 
	if (configInfo == nullptr)
	{
		qWarning() << "config info is empty";
		return false;
	}
	if (_socketBase)
	{
		destoryResources();
	}
	_socketBase = new FTcpClient;
	_socketBase->setIpAddress(configInfo->getIpAddress());
	_socketBase->setPort(configInfo->getPort());
	_socketBase->tryToStart();//之前放在  _threadSocket->start(); 之后 出错
	_threadSocket = new QThread; // 
	_socketBase->moveToThread(_threadSocket);//
	_threadSocket->start();
	connect(_socketBase, &FBaseSocket::sigSocketReceive, this, &FNetworkDebuggingAssistant::onReceiveMessage);
	connect(this, &FNetworkDebuggingAssistant::toSendMessage, _socketBase, &FBaseSocket::onSocketSend);

	return true;
}



bool FNetworkDebuggingAssistant::createTCPServerSocket(const FInfoConfig *configInfo)
{
	if (configInfo == nullptr)
	{
		qWarning() << "config info is empty";
		return false;
	}
	if (_socketBase)
	{
		destoryResources();
	}
	_socketBase = new FTcpServer;
	_socketBase->setIpAddress(configInfo->getIpAddress());
	_socketBase->setPort(configInfo->getPort());
	_socketBase->tryToStart();//之前放在  _threadSocket->start(); 之后 出错
	_threadSocket = new QThread; // 
	_socketBase->moveToThread(_threadSocket);//
	_threadSocket->start();
	connect(_socketBase, &FBaseSocket::sigSocketReceive, this, &FNetworkDebuggingAssistant::onReceiveMessage);
	connect(this, &FNetworkDebuggingAssistant::toSendMessage, _socketBase, &FBaseSocket::onSocketSend);
	connect(_socketBase, &FBaseSocket::sigNewConnection, this, &FNetworkDebuggingAssistant::onNewConnection);
	return true;
}

void FNetworkDebuggingAssistant::initSkin()
{
	//QMap<QSharedPointer<QAction>, QString> _skinAction2SkinFileName;
	 _acitonSkinBlack = QSharedPointer<QAction>::create(this);
	 _acitonSkinBlack->setText(W2Q(L"黑色"));
	 _acitonSkinGreen = QSharedPointer<QAction>::create(this);
	 _acitonSkinGreen->setText(W2Q(L"绿色"));
	 _acitonSkinRed = QSharedPointer<QAction>::create(this);
	 _acitonSkinRed->setText(W2Q(L"红色"));
	 _menuSkin = QSharedPointer<QMenu>::create(this);
	 _actionSkin = QSharedPointer<QAction>::create(this);
	 _menuSkin->addAction(_acitonSkinBlack.data());
	 _menuSkin->addAction(_acitonSkinGreen.data());
	 _menuSkin->addAction(_acitonSkinRed.data());
	 _actionSkin = QSharedPointer<QAction>::create(this);
	 _actionSkin->setText(W2Q(L"皮肤"));
	 _sysMenu->addAction(_actionSkin.data());
	 _actionSkin->setMenu(_menuSkin.data());
	 _skinAction2SkinFileName.insert(_acitonSkinBlack.data(), W2Q(L"data/skin/black/black.css"));
	 _skinAction2SkinFileName.insert(_acitonSkinGreen.data(), W2Q(L"data/skin/green/green.css"));
	 _skinAction2SkinFileName.insert(_acitonSkinRed.data(), W2Q(L"data/skin/red/red.css"));
	 connect(_acitonSkinBlack.data(), &QAction::triggered, this, &FNetworkDebuggingAssistant::onSkinChange);
	 connect(_acitonSkinGreen.data(), &QAction::triggered, this, &FNetworkDebuggingAssistant::onSkinChange);
	 connect(_acitonSkinRed.data(), &QAction::triggered, this, &FNetworkDebuggingAssistant::onSkinChange);
	 this->setStyleSheet(getSkinFilePathBySkinAction(_acitonSkinBlack.data()));//初始化为黑色皮肤
	 _actionCurrentSkin = _acitonSkinBlack.data();
}

