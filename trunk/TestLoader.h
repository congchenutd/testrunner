#ifndef TESTLOADER_H
#define TESTLOADER_H

#include <QFont>

class QXmlStreamReader;

class TestLoader
{
public:
	void setXML(QXmlStreamReader* x) { xml = x; }
	void  loadStyle() const;

private:
	QFont loadFont() const;

private:
	QXmlStreamReader* xml;
};

#endif // TESTLOADER_H
