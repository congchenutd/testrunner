#ifndef PAGEFACTORY_H
#define PAGEFACTORY_H

class TestPage;
class QString;
class QTime;

class PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end) = 0;

	static getFactory(const QString& factoryName);
};

class TextPageFactory : public PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end);
};

class IntegerPageFactory : public PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end);
};

class SingleChoicePageFactory : public PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end);
};

class MutipleChoicePageFactory : public PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end);
};

class BlankFillingPageFactory : public PageFactory
{
public:
	virtual TestPage* createPage(const QString& title, const QString& text,
								 bool skip, int min, int max, bool timeIt,
								 const QTime& start, const QTime& end);
};

#endif // PAGEFACTORY_H
