#include "FIrregularWidget.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	FIrregularWidget wa;
	wa.show();
	FIrregularMaskWidget w;
	w.show();
	return a.exec();
}