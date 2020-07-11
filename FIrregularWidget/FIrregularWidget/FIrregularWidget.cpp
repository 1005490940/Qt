#include "FIrregularWidget.h"
#include <QFile>
FIrregularWidget::FIrregularWidget(QWidget *parent /*= nullptr*/)
{
	initUi();
	readStyleSheetFile("skin/style.css");
	this->setWindowFlags(Qt::FramelessWindowHint | this->windowFlags());
	this->setAttribute(Qt::WA_TranslucentBackground);
}

FIrregularWidget::~FIrregularWidget()
{

}

void FIrregularWidget::initUi()
{
	_layoutMain = new QVBoxLayout;
	_layoutMain->setContentsMargins(0, 0, 0, 0);//±³¾°ÍêÈ«ÌùºÏ
	_widgetBackground = new QWidget;
	_widgetBackground->setObjectName("widgetBackground");
	_layoutMain->addWidget(_widgetBackground);
    setLayout(_layoutMain);
	this->setObjectName("FIrregularWidget");
}

void FIrregularWidget::readStyleSheetFile(const QString & fileName)
{
	QFile file(fileName);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	file.close();
}
FIrregularMaskWidget::FIrregularMaskWidget(QWidget *parent /*= nullptr*/)
{
	_backgroundPixmap = QPixmap("skin/test.png");
	setWidgetMask();
}

FIrregularMaskWidget::~FIrregularMaskWidget()
{

}

void FIrregularMaskWidget::initUi()
{


}

void FIrregularMaskWidget::readStyleSheetFile(const QString & fileName)
{
}

void FIrregularMaskWidget::setWidgetMask()
{
	resize(_backgroundPixmap.size());
	this->setMask(_backgroundPixmap.mask());
	
}

void FIrregularMaskWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), _backgroundPixmap.toImage());
}

void FIrregularMaskWidget::resizeEvent(QResizeEvent * event)
{
	setWidgetMask();
	return QWidget::resizeEvent(event);
}

