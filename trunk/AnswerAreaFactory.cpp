#include "AnswerAreaFactory.h"
#include "AnswerArea.h"
#include <QXmlStreamReader>

IAnswerArea* DefaultAreaFactory::load(QXmlStreamReader&) {
	return new DefaultAnswerArea;
}

IAnswerArea* SingleChoiceAreaFactory::load(QXmlStreamReader& xml)
{
	SingleChoiceArea* result = new SingleChoiceArea;
	while(xml.readNextStartElement() && xml.name() == "choice")
		result->addChoice(xml.readElementText());
	return result;
}

IAnswerArea* MultipleChoiceAreaFactory::load(QXmlStreamReader& xml)
{
	MultipleChoiceArea* result = new MultipleChoiceArea;
	while(xml.readNextStartElement() && xml.name() == "choice")
		result->addChoice(xml.readElementText());
	return result;
}

IAnswerArea* IntegerAreaFactory::load(QXmlStreamReader& xml)
{
	IntegerArea* result = new IntegerArea;
	int min = xml.attributes().value("min").toString().toInt();
	int max = xml.attributes().value("max").toString().toInt();
	result->setValueRange(min, max);
	return result;
}

IAnswerArea* BlankFillingAreaFactory::load(QXmlStreamReader& xml)
{
	BlankFillingArea* result = new BlankFillingArea;
	while(xml.readNextStartElement() && xml.name() == "line")
		result->addBlank(xml.readElementText());
	return result;
}
