#ifndef ANSWERAREAFACTORY_H
#define ANSWERAREAFACTORY_H

#include <QObject>

class AnswerArea;
class QXmlStreamReader;

class AnswerAreaFactory : public QObject
{
public:
	AnswerAreaFactory(QObject* parent = 0) :QObject(parent) {}
	virtual ~AnswerAreaFactory() {}
	virtual AnswerArea* load(QXmlStreamReader& xml) = 0;
};

class DefaultAreaFactory : public AnswerAreaFactory
{
public:
	DefaultAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class SingleChoiceAreaFactory : public AnswerAreaFactory
{
public:
	SingleChoiceAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class MultipleChoiceAreaFactory : public AnswerAreaFactory
{
public:
	MultipleChoiceAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class IntegerAreaFactory : public AnswerAreaFactory
{
public:
	IntegerAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

class BlankFillingAreaFactory : public AnswerAreaFactory
{
public:
	BlankFillingAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual AnswerArea* load(QXmlStreamReader& xml);
};

#endif // ANSWERAREAFACTORY_H
