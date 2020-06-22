#pragma once
#include <QObject>
#include <QDomComment>
#include <QSharedPointer>
#include <QVector>
class FXmlAttribute
{
public:
	FXmlAttribute();
	~FXmlAttribute();
	void setAttributeName(const std::wstring &name);
	std::wstring getAttributeName()const;
	void setAttributeValue(const std::wstring &value);
	std::wstring getAttributeValue()const;
private:
	std::wstring _attributeName;
	std::wstring _attributeValue;
};
class FXmlNode
{
public:
	using  ChildNodeList = QVector< QSharedPointer<FXmlNode>>;//·ÀÖ¹ÄÚ´æÐ¹Â©
	FXmlNode();
	FXmlNode(const std::wstring &name, const std::wstring &value);
	FXmlNode(const FXmlNode &node);
	~FXmlNode();
private:
	std::wstring _nodeName;
	std::wstring _nodeValue;
	ChildNodeList _nodeList;
};
class FXmlDocument
{
public:
	FXmlDocument();
	~FXmlDocument();
	bool tryToImportXml(const std::wstring &path);
	bool tryToExportXml(const std::wstring &path);
protected:
	bool exportXml(const std::wstring &path);
	bool importXml(const std::wstring &path);
private:
};


