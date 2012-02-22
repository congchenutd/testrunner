#include "TestLoader.h"
#include "TestPage.h"
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
