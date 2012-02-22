#ifndef TESTLOADER_H
#define TESTLOADER_H

#include <QFont>

class QXmlStreamReader;
class TestPage;
class MainWindow;

// Parse the xml file and create pages
class TestLoader : public QObject
{
public:
	TestLoader(MainWindow* mainWnd) : mainWindow(mainWnd) {}
	void      setXML(QXmlStreamReader* x) { xml = x; }
	void      loadStyle() const;
	TestPage* loadInitialization();              // anything before sections
	TestPage* loadNext();                        // next question or section
	TestPage* loadIntro(const QString& title);   // intro of a section or test
	TestPage* loadSection();
	TestPage* loadQuestion();

private:
	QFont loadFont() const;

private:
	QXmlStreamReader* xml;
	MainWindow* mainWindow;
};

#endif // TESTLOADER_H
