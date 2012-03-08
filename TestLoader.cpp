#include "TestLoader.h"
#include "TestPage.h"
#include "TestPage.h"
#include "MainWindow.h"
#include "AnswerAreaFactory.h"
#include <QApplication>

bool TestLoader::openXMLFile(QFile& xmlFile)
{
	xml.setDevice(&xmlFile);
	return xml.readNextStartElement() && xml.name() == "test";
}

bool TestLoader::atEnd() {
	return xml.atEnd();
}

// A font section may contain family, weight and size
QFont TestLoader::loadFont()
{
	QFont result = qApp->font();        // use system font by default
	if(!xml.readNextStartElement() || xml.name() != "font")
		return result;

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

	// till </style>
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
	while(!xml.atEnd())
	{
		if(xml.isStartElement())
		{
			if(xml.name() == "style")
				loadStyle();                   // does not return
			else if(xml.name() == "intro")
				return loadIntro(tr("Introduction"));
			if(xml.name() == "section")        // a new section
				return loadSection();
			else if(xml.name() == "question")  // next question in the same section
				return loadQuestion();
		}
		xml.readNextStartElement();
	}
	return loadEndPage();
}

// a test or a section can have a introduction
TestPage* TestLoader::loadIntro(const QString& title)
{
	if(!xml.isStartElement() || xml.name() != "intro")
		return 0;

	QString introText = xml.readElementText();
	return introText.isEmpty() ? 0 : new TestPage(title, introText);
}

TestPage* TestLoader::loadSection()
{
	if(!xml.isStartElement() || xml.name() != "section")
		return 0;

	QString title = xml.attributes().value("title").toString();
	if(title.isEmpty())
		title = tr("Section");
	mainWindow->setTitle(title);

	if(xml.readNextStartElement()) {
		if(xml.name() == "intro")
			return loadIntro(title);
		else if(xml.name() == "question")
			return loadQuestion();
	}
	return 0;
}

TestPage* TestLoader::loadQuestion()
{
	if(!xml.isStartElement() || xml.name() != "question")
		return 0;

	// attributes of the question
	QString title   = xml.attributes().value("title")  .toString();
	bool    maySkip = xml.attributes().value("mayskip").toString() == "true";
	bool    timeIt  = xml.attributes().value("timeit") .toString() == "true";
	bool    isName  = xml.attributes().value("isname") .toString() == "true";

	if(!xml.readNextStartElement() || xml.name() != "content")
		return 0;
	QString text = xml.readElementText();
	if(text.isEmpty())
		return 0;

	if(!xml.readNextStartElement() || xml.name() != "answer")
		return 0;
	if(!xml.readNextStartElement())
		return 0;
	mainWindow->setAnswered(true);   // actually, just loaded, not really answered

	// prepare the page with the question and the answer area
	TestPage* page = new TestPage(title, text);
	page->setSkippable(maySkip);
	page->setTimerEnabled(timeIt);
	page->setIsName(isName);

	QString pageName = xml.name().toString();
	page->setAnswerArea(createAnswerAreaFactory(pageName)->load(xml));
	return page;
}

TestPage *TestLoader::loadEndPage()
{
	mainWindow->setTitle(tr("Finished"));
	return new TestPage(tr("Finished"),
						tr("Thank you for your cooperation! You may quit the test now."));
}

AnswerAreaFactory* TestLoader::createAnswerAreaFactory(const QString& factoryName)
{
	if(factoryName == "single")
		return new SingleChoiceAreaFactory;
	else if(factoryName == "multiple")
		return new MultipleChoiceAreaFactory;
	else if(factoryName == "integer")
		return new IntegerAreaFactory;
	else if(factoryName == "blank")
		return new BlankFillingAreaFactory;
	return new DefaultAreaFactory;
}
