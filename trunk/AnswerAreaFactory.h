#ifndef ANSWERAREAFACTORY_H
#define ANSWERAREAFACTORY_H

#include <QObject>

class IAnswerArea;
class QXmlStreamReader;

class AnswerAreaFactory : public QObject
{
public:
	AnswerAreaFactory(QObject* parent = 0) :QObject(parent) {}
	virtual ~AnswerAreaFactory() {}
	virtual IAnswerArea* load(QXmlStreamReader& xml) = 0;
};

class DefaultAreaFactory : public AnswerAreaFactory
{
public:
	DefaultAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual IAnswerArea* load(QXmlStreamReader& xml);
};

class SingleChoiceAreaFactory : public AnswerAreaFactory
{
public:
	SingleChoiceAreaFactory(QObject* parent = 0) :AnswerAreaFactory(parent) {}
	virtual IAnswerArea* load(QXmlStreamReader& xml);
};

#endif // ANSWERAREAFACTORY_H
