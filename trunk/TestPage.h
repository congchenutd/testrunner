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

// TestPage is like QWizardPage, hosting a question and an area for answer
// By default, it only contains a title and a line of text for the question
class TestPage : public QWidget
{
	Q_OBJECT

public:
	TestPage();
	virtual ~TestPage() {}

	virtual void setTitle(const QString& title);
	virtual void setText (const QString& text);
	virtual void setSkippable   (bool skip);
	virtual void setTimerEnabled(bool enable);
	virtual void setIsName      (bool name);
	virtual void setDuration(const QTime& start, const QTime& end);
	virtual void setValueRange(int min, int max) {}    // for IntegerPage
	virtual void addChoice(const QString& choice) {}   // for single/multiple choice page

	virtual QVariant getAnswer() const = 0;
	virtual void setFocus();   // allow derived to set focus to widgets of input

	QString toString() const;
	bool    isNamePage() const { return isName; }

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
	QLabel leTitle;
	QLabel leText;
	bool maySkip;   // Is this page optional
	int  elapsed;   // How long (seconds) was spent on this page
	bool isName;    // Does this page contains participant name?
	QTime startTime;
	QTime endTime;

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

private:
	QLineEdit* lineEdit;
};

#endif // TESTPAGE_H
