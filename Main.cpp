#include <QtGui/QApplication>
#include "MainWindow.h"
#include "TestState.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWnd;

    bool loaded = false;
	if(argc == 1) {
		// Load TestRunner.xml if it is placed at the same directory
        loaded = QFile::exists("TestRunner.xml") ? mainWnd.init("TestRunner.xml")
                                                 : mainWnd.init();
	}
	else {
        loaded = mainWnd.init(argv[1]);
	}

    if(loaded)
	{
		mainWnd.show();
		return app.exec();
	}
	return 1;
}
