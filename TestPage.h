#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWidget>
#include <QVariant>
#include <QList>
#include <QLabel>
#include <QFont>
#include <QTime>

class QRadioButton;
class QCheckBox;
class QSpinBox;
class QLineEdit;
class IAnswerArea;

// TestPage is like QWizardPage, hosting a question and an area for answer
// By default, it only contains a title and a line of text for the question
class TestPage : public QWidget
{
	Q_OBJECT

public:
	TestPage(const QString& title, const QString& text,
			 bool skip = false, bool timer = false, bool name = false);

	void setTitle(const QString& title);
	void setText (const QString& text);
	void setSkippable   (bool skip) { maySkip = skip; }
	void setTimerEnabled(bool enable);
	void setAnswerArea(IAnswerArea* answer);
	void setIsName (bool name) { isName = name; }
	bool isNamePage() const    { return isName; }

	QString  toString() const;
	QVariant getAnswer() const;

	static void setGlobalFont(const QFont& font) { globalFont = font; }
	static void setTitleFont (const QFont& font) { titleFont  = font; }
	static void setTextFont  (const QFont& font) { textFont   = font; }

public slots:
	virtual bool validate() const;        // validate the answer

signals:
	void valid(bool) const;               // result of validate()

private:
	bool accept(bool ok) const;           // for convenience

private slots:
	void onTimer();

private:
	QLabel* leTitle;
	QLabel* leText;
	bool maySkip;     // Is this page optional
	int  elapsed;     // How long (seconds) was spent on this page
	bool isName;
	IAnswerArea* answerArea;

	static QFont globalFont;   // style
	static QFont titleFont;
	static QFont textFont;
};

#endif // TESTPAGE_H
