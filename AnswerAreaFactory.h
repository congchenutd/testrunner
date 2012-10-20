#ifndef ANSWERAREAFACTORY_H
#define ANSWERAREAFACTORY_H

#include <QObject>

class AnswerArea;
class QXmlStreamReader;

// build answerarea from xml
class AnswerAreaLoader : public QObject
{
public:
	AnswerAreaLoader(QObject* parent = 0) :QObject(parent) {}
	virtual ~AnswerAreaLoader() {}
	virtual AnswerArea* load(QXmlStreamReader& xml) = 0;

    static AnswerAreaLoader* getLoader(const QString& name);   // a simple factory
};

class DefaultAreaLoader : public AnswerAreaLoader
{
public:
    DefaultAreaLoader(QObject* parent = 0) :AnswerAreaLoader(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class SingleChoiceAreaLoader : public AnswerAreaLoader
{
public:
    SingleChoiceAreaLoader(QObject* parent = 0) :AnswerAreaLoader(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class MultipleChoiceAreaLoader : public AnswerAreaLoader
{
public:
    MultipleChoiceAreaLoader(QObject* parent = 0) :AnswerAreaLoader(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class IntegerAreaLoader : public AnswerAreaLoader
{
public:
    IntegerAreaLoader(QObject* parent = 0) :AnswerAreaLoader(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class BlankFillingAreaLoader : public AnswerAreaLoader
{
public:
    BlankFillingAreaLoader(QObject* parent = 0) :AnswerAreaLoader(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

#endif // ANSWERAREAFACTORY_H
