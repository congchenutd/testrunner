#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWidget>
#include <QVariant>
#include <QList>
#include <QLabel>
#include <QFont>
#include <QTime>
#include "AnswerArea.h"

class QRadioButton;
class QCheckBox;
class QSpinBox;
class QLineEdit;
class AnswerArea;

// TestPage is like QWizardPage, hosting a question and an area for answer
// By default, it only contains a title and a line of text for the question
class TestPage : public QWidget
{
	Q_OBJECT

public:
	TestPage(const QString& title, const QString& text,
			 bool skip = false, bool name = false);

	void setTitle(const QString& title);
	void setText (const QString& text);
	void setSkippable   (bool skip) { maySkip = skip; }
	void setAnswerArea(AnswerArea* answer);
	void setIsName (bool name) { isName = name; }
	bool isNamePage() const    { return isName; }

	QString  toString() const;
	QVariant getAnswer() const;
	void validate();                 // validate the answer

	static void setGlobalFont(const QFont& font) { globalFont = font; }
	static void setTitleFont (const QFont& font) { titleFont  = font; }
	static void setTextFont  (const QFont& font) { textFont   = font; }

signals:
	void statusChanged(AnswerStatus);  // result of validate()

private:
	QLabel* leTitle;
	QLabel* leText;
	bool maySkip;     // Is this page optional
	bool isName;
	AnswerArea* answerArea;

	static QFont globalFont;   // style
	static QFont titleFont;
	static QFont textFont;
};

#endif // TESTPAGE_H
