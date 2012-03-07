#include "AnswerAreaFactory.h"
#include "AnswerArea.h"
#include <QXmlStreamReader>

IAnswerArea* DefaultAreaFactory::load(QXmlStreamReader& xml) {
	return new DefaultArea;
}

IAnswerArea* SingleChoiceAreaFactory::load(QXmlStreamReader& xml)
{
	SingleChoiceArea* result = new SingleChoiceArea;
	while(xml.readNextStartElement() && xml.name() == "choice")
		result->addChoice(xml.readElementText());
	return result;
}
