#include "FNetworkDebuggingAssistant.h"
#include <QtWidgets/QApplication>
#include <windows.h>
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	const char *file = context.file ? context.file : "";
	const char *function = context.function ? context.function : "";
	switch (type) {
	case QtDebugMsg://debug info
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		fprintf(stderr, "Debug: %s  --File:(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		break;
	case QtWarningMsg: // warn info 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		fprintf(stderr, "Warning: %s  --File:(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		break;
	case QtCriticalMsg://error  info
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		fprintf(stderr, "Critical: %s --File:(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		break;
	}
}
int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    FNetworkDebuggingAssistant w;
    w.show();
    return a.exec();
}
