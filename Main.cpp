#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow mainWnd;

	// Load TestRunner.xml if it is placed at the same directory
	bool initResult = (QFile::exists("TestRunner.xml"))
					  ? mainWnd.init("TestRunner.xml")
					  : mainWnd.init();

	if(initResult)
	{
		mainWnd.show();
		return app.exec();
	}
	return 1;
}
