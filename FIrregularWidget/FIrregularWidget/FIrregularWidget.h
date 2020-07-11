#pragma once

#include <QBitmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QLabel>
class  FIrregularWidget : public QWidget
{
	Q_OBJECT
public:
    FIrregularWidget(QWidget *parent = nullptr);
	~FIrregularWidget();
private:
	void initUi();
	void readStyleSheetFile(const QString &file);
private:
	QWidget* _widgetBackground;
	QVBoxLayout *_layoutMain;
};

class  FIrregularMaskWidget : public QWidget
{
	Q_OBJECT
public:
	FIrregularMaskWidget(QWidget *parent = nullptr);
	~FIrregularMaskWidget();
private:
	void initUi();
	void readStyleSheetFile(const QString &file);
	void setWidgetMask();
protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
private:
	QPixmap _backgroundPixmap;
};