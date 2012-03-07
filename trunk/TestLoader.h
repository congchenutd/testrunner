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
	void      loadStyle();
	TestPage* loadNext();                        // next element
	TestPage* loadIntro(const QString& title);   // intro of a section or test
	TestPage* loadSection();
	TestPage* loadQuestion();
	TestPage* loadEndPage();

private:
	QFont loadFont();
	TestPage* createPage(const QString& pageName);
	TestPage* createTextPage(const QString& title, const QString& text);  // for convenience
	AnswerAreaFactory* createAnswerAreaFactory(const QString& factoryName);

private:
	QXmlStreamReader xml;
	MainWindow* mainWindow;
};



#endif // TESTLOADER_H
