#include <QtGui/QApplication>
#include "MainWindow.h"
#include "TestState.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWnd;

	bool initResult = false;
	if(argc == 1) {
		// Load TestRunner.xml if it is placed at the same directory
		initResult = QFile::exists("TestRunner.xml") ? mainWnd.init("TestRunner.xml")
													 : mainWnd.init();
	}
	else {
		initResult = mainWnd.init(argv[1]);
	}

	if(initResult)
	{
		mainWnd.show();
		return app.exec();
	}
	return 1;
}
