#include "FXmlDocument.h"
#include <QDebug>
FXmlDocument::FXmlDocument()
{

}

FXmlDocument::~FXmlDocument()
{

}

bool FXmlDocument::tryToImportXml(const std::wstring &path)
{
	return importXml(path);
}

bool FXmlDocument::tryToExportXml(const std::wstring &path)
{
	return exportXml(path);
}

bool FXmlDocument::exportXml(const std::wstring &path)
{
	if (path.empty())
	{
		qWarning() << "xml path is empty";
		return false;
	}
   
	return true;
}

bool FXmlDocument::importXml(const std::wstring &path)
{
	if (path.empty())
	{
		qWarning() << "xml path is empty";
		return false;
	}

	return true;
}

FXmlAttribute::FXmlAttribute()
	:_attributeName(L"")
	,_attributeValue(L"")
{

}

FXmlAttribute::~FXmlAttribute()
{

}

void FXmlAttribute::setAttributeName(const std::wstring &name)
{
     if (name.empty())
     {
		 qDebug() << "attribute name is empty";
		 return;
     }
	 _attributeName = name;
}

std::wstring FXmlAttribute::getAttributeName() const
{
	return _attributeName;
}

void FXmlAttribute::setAttributeValue(const std::wstring &value)
{
	if (value.empty())
	{
		qDebug() << "attribute name is empty";
		return;
	}
	_attributeValue = value;
}

std::wstring FXmlAttribute::getAttributeValue() const
{
	return _attributeValue;
}

FXmlNode::FXmlNode()
{

}

FXmlNode::FXmlNode(const std::wstring &name, const std::wstring &value)
	:_nodeName(name)
	,_nodeValue(value)
{

}

FXmlNode::FXmlNode(const FXmlNode &node)
	:_nodeName(node._nodeName)
	,_nodeValue(node._nodeValue)
{

}

FXmlNode::~FXmlNode()
{

}
