#pragma once
#include <QString>
#include <string.h>
class FQStringUtil
{
public:
	static std::string QStringToStdString(const QString &qstr);
	static QString StdStringToQString(const std::string &sstr);
	static std::wstring QStringToWStdString(const QString &qstr);
	static QString WStdStringToQString(const std::wstring &sstr);
};
#define  S2Q(str)  FQStringUtil::StdStringToQString(str)
#define  Q2S(str)  FQStringUtil::QStringToStdString(str)
#define  W2Q(str)  FQStringUtil::WStdStringToQString(str)
#define  Q2W(str)  FQStringUtil::QStringToWStdString(str)
