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
	TestPage();
	virtual ~TestPage() {}

	void setTitle(const QString& title);
	void setText (const QString& text);
	void setSkippable   (bool skip) { maySkip = skip; }
	void setTimerEnabled(bool enable);
	QString toString() const;
	void setAnswerArea(IAnswerArea* answerArea);
	void finalize();

	virtual void setIsName    (bool)           {}      // for BlankFillingPage
	virtual void setValueRange(int, int)       {}      // for IntegerPage
	virtual void addChoice    (const QString&) {}      // for single/multiple choice page
	virtual bool isNamePage() const { return false; }  // for BlankFillingPage

	virtual QVariant getAnswer() const = 0;
	virtual void setFocus();   // allow derived to set focus to input widgets

	static void setGlobalFont(const QFont& font) { globalFont = font; }
	static void setTitleFont (const QFont& font) { titleFont  = font; }
	static void setTextFont  (const QFont& font) { textFont   = font; }

public slots:
	virtual bool validate() const;        // validate the answer

signals:
	void valid(bool) const;               // result of validate()

protected:
	bool accept(bool ok) const;           // for convenience
	virtual void showEvent(QShowEvent*);

private slots:
	void onTimer();

private:
	QLabel* leTitle;
	QLabel* leText;
	bool maySkip;     // Is this page optional
	int  elapsed;     // How long (seconds) was spent on this page

	static QFont globalFont;   // style
	static QFont titleFont;
	static QFont textFont;
};

// nothing but title and text inherited from TestPage
class TextPage : public TestPage
{
public:
	virtual QVariant getAnswer() const { return QString(); }
	virtual bool validate() const;   // always true
};

class SingleChoicePage : public TestPage
{
public:
	virtual QVariant getAnswer() const;
	virtual void addChoice(const QString& choice);
	virtual void setFocus();

private:
	QList<QRadioButton*> radioButtons;
};

class MultipleChoicePage : public TestPage
{
public:
	virtual QVariant getAnswer() const;
	virtual void addChoice(const QString& choice);
	virtual void setFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class IntegerPage : public TestPage
{
public:
	IntegerPage();

	virtual QVariant getAnswer() const;
	virtual void setValueRange(int min, int max);
	virtual void setFocus();

private:
	QSpinBox* spinBox;
};

class BlankFillingPage : public TestPage
{
public:
	BlankFillingPage();

	virtual QVariant getAnswer() const;
	virtual void setFocus();
	virtual void setIsName(bool name) { isName = name; }
	virtual bool isNamePage() const { return isName; }

private:
	QLineEdit* lineEdit;
	bool isName;            // Does this page contains participant name?
};

#endif // TESTPAGE_H
