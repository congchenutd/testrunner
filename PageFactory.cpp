#include "PageFactory.h"
#include "TestPage.h"

PageFactory::getFactory(const QString& factoryName)
{
	if(factoryName == "text")
		return new TextPageFactory;
	else if(factoryName == "integer")
		return new IntegerPageFactory;
	else if(factoryName == "single")
		return new SingleChoicePageFactory;
	else if(factoryName == "multiple")
		return new MultileChoicePageFactory;
	else if(factoryName == "blank")
		return new BlankFillingPageFactory;
	return new TextPageFactory;
}

TestPage *TextPageFactory::createPage(const QString& title, const QString& text,
									  bool skip, int min, int max, bool timeIt,
									  const QTime& start, const QTime& end)
{
	return new TextPage(title, text);
}

TestPage *IntegerPageFactory::createPage(const QString& title, const QString& text,
										 bool skip, int min, int max, bool timeIt,
										 const QTime& start, const QTime& end)
{
	return 0;
}

TestPage *SingleChoicePageFactory::createPage(const QString& title, const QString& text,
											  bool skip, int min, int max, bool timeIt,
											  const QTime& start, const QTime& end)
{
	return 0;
}

TestPage *MutipleChoicePageFactory::createPage(const QString& title, const QString& text,
											   bool skip, int min, int max, bool timeIt,
											   const QTime& start, const QTime& end)
{
	return 0;
}

TestPage *BlankFillingPageFactory::createPage(const QString& title, const QString& text,
											  bool skip, int min, int max, bool timeIt,
											  const QTime& start, const QTime& end)
{
	return 0;
}

