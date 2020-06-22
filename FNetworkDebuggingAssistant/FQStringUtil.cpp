#include "FQStringUtil.h"

std::string FQStringUtil::QStringToStdString(const QString &qstr)
{
	return qstr.toStdString();
}

QString FQStringUtil::StdStringToQString(const std::string &sstr)
{
	return  QString::fromStdString(sstr);
}

std::wstring FQStringUtil::QStringToWStdString(const QString &qstr)
{
	return qstr.toStdWString();
}

QString FQStringUtil::WStdStringToQString(const std::wstring &sstr)
{
	return  QString::fromStdWString(sstr);
}
