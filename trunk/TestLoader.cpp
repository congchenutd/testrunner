#include "TestLoader.h"
#include "TestPage.h"
#include "TestPage.h"
#include "AnswerAreaFactory.h"
#include <QApplication>
#include <QFile>

bool TestLoader::openXMLFile(QFile& xmlFile)
{
	xml.setDevice(&xmlFile);
    end = false;
	return xml.readNextStartElement() && xml.name() == "test";
}

bool TestLoader::atEnd() {
    return end || xml.atEnd();
}

QFont TestLoader::loadFont()
{
	QFont result = qApp->font();        // use system font by default
	if(!xml.readNextStartElement() || xml.name() != "font")
		return result;

    // A font section may contain family, weight and size
    // untill </font>
	while(!(xml.isEndElement() && xml.name() == "font")) {
		if(xml.readNextStartElement())
		{
			if(xml.name() == "family")
				result.setFamily(xml.readElementText());
			else if(xml.name() == "weight")
			{
				QString weight = xml.readElementText();
				if(weight == "normal")
					result.setWeight(QFont::Normal);
				else if(weight == "bold")
					result.setWeight(QFont::Bold);
			}
			else if(xml.name() == "size")
				result.setPointSize(xml.readElementText().toInt());
		}
	}

	return result;
}

void TestLoader::loadStyle()
{
	if(!xml.isStartElement() || xml.name() != "style")
		return;

    // untill </style>
	while(!(xml.isEndElement() && xml.name() == "style")) {
		if(xml.readNextStartElement())
		{
			if(xml.name() == "global")
				TestPage::setGlobalFont(loadFont());
			else if(xml.name() == "title")
				TestPage::setTitleFont(loadFont());
			else if(xml.name() == "text")
				TestPage::setTextFont(loadFont());
		}
	}
}

TestPage* TestLoader::loadNext()
{
    while(!atEnd()) {
        if(xml.readNextStartElement())
        {
            if(xml.name() == "style")
                loadStyle();                   // does not return a page
            else if(xml.name() == "section")   // a new section
                return loadSection();
            else if(xml.name() == "question")  // next question in the same section
                return loadQuestion();
        }
    }
    return loadEndPage();
}

TestPage* TestLoader::loadSection()
{
    if(!xml.isStartElement() || xml.name() != "section")
        return loadErrorPage(tr("There should be a section here!"));

    end = xml.attributes().value("end").toString() == "true";  // the last section
    QString title = xml.attributes().value("title").toString();
    if(title.isEmpty())
        title = tr("Section");
    emit titleChanged(title);

    if(xml.readNextStartElement()) {
        if(xml.name() == "intro")
            return loadIntro(title);
        else if(xml.name() == "question")  // if no intro, then load question
            return loadQuestion();
    }
    return loadErrorPage(tr("Something wrong with this section!"));
}

TestPage* TestLoader::loadIntro(const QString& title)
{
	if(!xml.isStartElement() || xml.name() != "intro")
        return loadErrorPage(tr("There should be an intro element here!"));
    return new TestPage(title, xml.readElementText());
}

TestPage* TestLoader::loadQuestion()
{
	if(!xml.isStartElement() || xml.name() != "question")
        return loadErrorPage(tr("There should be a question here!"));

	// attributes of the question
	QString title   = xml.attributes().value("title")  .toString();
	bool    maySkip = xml.attributes().value("mayskip").toString() == "true";
	bool    isName  = xml.attributes().value("isname") .toString() == "true";

	if(!xml.readNextStartElement() || xml.name() != "content")
        return loadErrorPage(tr("There should be a content here!"));
	QString questionText = xml.readElementText();
	if(questionText.isEmpty())
        return loadErrorPage(tr("The question is empty!"));

	// create the page with the question
	TestPage* page = new TestPage(title, questionText);
	page->setSkippable(maySkip);
	page->setIsName(isName);

	// create the answer area of the page
	if(!xml.readNextStartElement() || xml.name() != "answer")
		return page;
	if(!xml.readNextStartElement())
		return page;
	QString answerType = xml.name().toString();
    page->setAnswerArea(AnswerAreaLoader::getLoader(answerType)->load(xml));
	return page;
}

TestPage* TestLoader::loadEndPage()
{
    emit titleChanged(tr("Finished"));
    return new TestPage(tr("Finished"), tr("You may quit now."));
}

TestPage* TestLoader::loadErrorPage(const QString& msg) {
    return new TestPage(tr("Error"), tr("Line %1: ").arg(xml.lineNumber()) + msg);
}


