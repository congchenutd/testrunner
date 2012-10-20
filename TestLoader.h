#ifndef TESTLOADER_H
#define TESTLOADER_H

#include <QFont>
#include <QXmlStreamReader>

class TestPage;
class QFile;
class AnswerAreaLoader;

// Parse the xml file and create pages
class TestLoader : public QObject
{
    Q_OBJECT

public:
    TestLoader(QObject* parent = 0) : QObject(parent), end(false) {}
	bool      openXMLFile(QFile& fileName);
    bool      atEnd();         // at the end of xml or seen the "end" section
    TestPage* loadNext();      // next xml element

private:
	void      loadStyle();
    QFont     loadFont();
    TestPage* loadSection();
    TestPage* loadIntro(const QString& title);   // intro of a section
    TestPage* loadQuestion();
    TestPage* loadEndPage();     // the default end page if xml doesn't have one
    TestPage* loadErrorPage(const QString& msg);

signals:
    void titleChanged(const QString&);

private:
	QXmlStreamReader xml;
    bool end;                // seen the "end" section?
};

#endif // TESTLOADER_H
