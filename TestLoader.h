#ifndef TESTLOADER_H
#define TESTLOADER_H

#include <QFont>
#include <QXmlStreamReader>

class TestPage;
class MainWindow;
class QFile;
class AnswerAreaFactory;

// Parse the xml file and create pages
class TestLoader : public QObject
{
public:
	TestLoader(MainWindow* mainWnd) : mainWindow(mainWnd) {}
	bool      openXMLFile(QFile& fileName);
	bool      atEnd();
	TestPage* loadNext();                        // next element
	TestPage* loadQuestion();

private:
	void      loadStyle();
	TestPage* loadIntro(const QString& title);   // intro of a section or test
	TestPage* loadSection();
	TestPage* loadEndPage();
	QFont     loadFont();
	AnswerAreaFactory* createAnswerAreaFactory(const QString& factoryName);

private:
	QXmlStreamReader xml;
	MainWindow* mainWindow;
};

#endif // TESTLOADER_H
