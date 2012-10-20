#include "AnswerAreaFactory.h"
#include "AnswerArea.h"
#include <QXmlStreamReader>

AnswerAreaLoader* AnswerAreaLoader::getLoader(const QString& name)
{
    static DefaultAreaLoader        defaultAreaLoader;
    static SingleChoiceAreaLoader   singleChoiceAreaLoader;
    static MultipleChoiceAreaLoader multipleChoiceAreaLoader;
    static IntegerAreaLoader        integerAreaLoader;
    static BlankFillingAreaLoader   blankFillingAreaLoader;

    if(name == "single")
        return &singleChoiceAreaLoader;
    else if(name == "multiple")
        return &multipleChoiceAreaLoader;
    else if(name == "integer")
        return &integerAreaLoader;
    else if(name == "blank")
        return &blankFillingAreaLoader;
    return &defaultAreaLoader;
}

AnswerArea* DefaultAreaLoader::load(QXmlStreamReader&) {
	return new DefaultAnswerArea;
}

AnswerArea* SingleChoiceAreaLoader::load(QXmlStreamReader& xml)
{
	SingleChoiceArea* result = new SingleChoiceArea;
	while(xml.readNextStartElement() && xml.name() == "choice")
		result->addChoice(xml.readElementText());
	return result;
}

AnswerArea* MultipleChoiceAreaLoader::load(QXmlStreamReader& xml)
{
	MultipleChoiceArea* result = new MultipleChoiceArea;
	while(xml.readNextStartElement() && xml.name() == "choice")
		result->addChoice(xml.readElementText());
	return result;
}

AnswerArea* IntegerAreaLoader::load(QXmlStreamReader& xml)
{
	int min = xml.attributes().value("min").toString().toInt();
	int max = xml.attributes().value("max").toString().toInt();
	IntegerArea* result = new IntegerArea(min, max, xml.readElementText());
	return result;
}

AnswerArea* BlankFillingAreaLoader::load(QXmlStreamReader& xml)
{
	BlankFillingArea* result = new BlankFillingArea;
	while(xml.readNextStartElement() && xml.name() == "line")
		result->addBlank(xml.readElementText());
	return result;
}
