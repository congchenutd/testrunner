#include "TestLoader.h"
#include "TestPage.h"
#include "TestPage.h"
#include "MainWindow.h"
#include <QXmlStreamReader>
#include <QApplication>

// A font section must contain fontfamily, fontweight and fontsize
QFont TestLoader::loadFont() const
{
	QFont result = qApp->font();        // use system font by default
	if(xml->readNextStartElement() && xml->name() == "fontfamily")
		result.setFamily(xml->readElementText());
	if(xml->readNextStartElement() && xml->name() == "fontweight") {
		if(xml->readElementText() == "normal")
			result.setWeight(QFont::Normal);
		else if(xml->readElementText() == "bold")
			result.setWeight(QFont::Bold);
	}
	if(xml->readNextStartElement() && xml->name() == "fontsize")
		result.setPointSize(xml->readElementText().toInt());
	return result;
}

void TestLoader::loadStyle() const
{
	if(!xml->isStartElement() || xml->name() != "style")
		return;

	while(!(xml->isEndElement() && xml->name() == "style")) {
		if(xml->readNextStartElement())
		{
			if(xml->name() == "global")
				TestPage::setGlobalFont(loadFont());
			else if(xml->name() == "title")
			{
				QString n = xml->name().toString();
				TestPage::setTitleFont(loadFont());
			}
			else if(xml->name() == "text")
				TestPage::setTextFont(loadFont());
		}
	}
}

TestPage *TestLoader::loadInitialization()
{
	while(xml->readNextStartElement())
	{
		if(xml->name() == "style")
			loadStyle();
		else if(xml->name() == "intro")
			return loadIntro(tr("Introduction"));
		else if(xml->name() == "section")
			return loadNext();
	}
	return 0;
}

TestPage *TestLoader::loadNext()
{
	while(!xml->atEnd())
	{
		if(xml->isStartElement()) {
			if(xml->name() == "section")        // a new section
				return loadSection();
			else if(xml->name() == "question")  // next question in the same section
				return loadQuestion();
		}
		xml->readNextStartElement();
	}
	return 0;
}

// a test or a section can have a introduction
TestPage* TestLoader::loadIntro(const QString& title)
{
	if(!xml->isStartElement() || xml->name() != "intro")
		return 0;

	QString introText = xml->readElementText();
	if(introText.isEmpty())
		return 0;

	return new TextPage(title, introText);
}

TestPage* TestLoader::loadSection()
{
	if(!xml->isStartElement() || xml->name() != "section")
		return 0;

	QString title = xml->attributes().value("title").toString();
	if(title.isEmpty())
		title = tr("Section");
	mainWindow->setTitle(title);

	if(xml->readNextStartElement()) {
		if(xml->name() == "intro")
			return loadIntro(title);
		else if(xml->name() == "question")
			return loadQuestion();
	}
	return 0;
}

TestPage* TestLoader::loadQuestion()
{
	if(!xml->isStartElement() || xml->name() != "question")
		return 0;

	QString title   = xml->attributes().value("title")  .toString();
	bool    maySkip = xml->attributes().value("mayskip").toString() == "true";
	bool    timeIt  = xml->attributes().value("timeit") .toString() == "true";
	bool    isName  = xml->attributes().value("isname") .toString() == "true";
//	QString start   = xml->attributes().value("start")  .toString();
//	QString end     = xml->attributes().value("end")    .toString();

	if(!xml->readNextStartElement() || xml->name() != "content")
		return 0;

	QString content = xml->readElementText();
	if(content.isEmpty())
		return 0;

	if(!xml->readNextStartElement() || xml->name() != "answer")
		return 0;

	if(!xml->readNextStartElement())
		return 0;

	mainWindow->setAnswered(true);
	if(xml->name() == "integer")
	{
		int min = xml->attributes().value("min").toString().toInt();
		int max = xml->attributes().value("max").toString().toInt();
		return new IntegerPage(min, max, title, content, maySkip, timeIt);
	}
	else if(xml->name() == "single")
	{
		SingleChoicePage* page   = new SingleChoicePage(title, content, maySkip, timeIt);
		while(xml->readNextStartElement() && xml->name() == "choice")
			page->addChoice(xml->readElementText());
		return page;
	}
	else if(xml->name() == "multiple")
	{
		MultipleChoicePage* page = new MultipleChoicePage(title, content, maySkip, timeIt);
		while(xml->readNextStartElement() && xml->name() == "choice")
			page->addChoice(xml->readElementText());
		return page;
	}
	else if(xml->name() == "blank") {
		return new BlankFillingPage(title, content, maySkip, timeIt, isName);
	}

	return 0;
}
