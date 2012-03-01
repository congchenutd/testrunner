#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

//	QWidget* widget = new QWidget;
//	QVBoxLayout* layout = new QVBoxLayout(widget);
//	QLabel* label = new QLabel("This is a survey designed to understand your previous experience with collaborative software development and proficiency with C++ and Qt Creator.");
//	label->setWordWrap(true);
//	layout->addWidget(label);
//	widget->show();

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


	return app.exec();
}
