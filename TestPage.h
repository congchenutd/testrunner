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
	TestPage() {}
	TestPage(const QString& ttl = QString(), const QString& tx = QString(),
			 bool skip = false, bool timeIt = false, bool name = false);
	virtual ~TestPage() {}

	void setTitle(const QString& title);
	void setText (const QString& text);
	void setSkippable   (bool skip);
	void setTimerEnabled(bool enable);
	void setIsName      (bool name);
	void setDuration(const QTime& start, const QTime& end);
	void setMinimum(int min) {}                      // valid for IntegerPage
	void setMaximum(int max) {}
	void setChoices(const QStringList& choices) {}   // valid for single/multiple choice page

	QString getTitle() const { return leTitle.text(); }
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
	TextPage(const QString& title = QString(), const QString& text = QString())
		: TestPage(title, text, true) {}

	virtual QVariant getAnswer() const { return QString(); }
	virtual bool validate() const;   // always true
};

class SingleChoicePage : public TestPage
{
public:
	SingleChoicePage(const QString& title = QString(), const QString& text = QString(),
					 bool skip = false, bool timeIt = false)
		: TestPage(title, text, skip, timeIt) {}

	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void setFocus();

private:
	QList<QRadioButton*> radioButtons;
};

class MultipleChoicePage : public TestPage
{
public:
	MultipleChoicePage(const QString& title = QString(), const QString& text = QString(),
					   bool skip = false, bool timeIt = false)
		: TestPage(title, text, skip, timeIt) {}

	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void setFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class IntegerPage : public TestPage
{
public:
	IntegerPage(int min, int max, const QString& leTitle = QString(),
				const QString& leText = QString(), bool skip = false, bool timeIt = false);

	virtual QVariant getAnswer() const;
	virtual void setFocus();

private:
	QSpinBox* spinBox;
};

class BlankFillingPage : public TestPage
{
public:
	BlankFillingPage(const QString& leTitle = QString(), const QString& leText = QString(),
					 bool skip = false, bool timeIt = false, bool isName = false);

	virtual QVariant getAnswer() const;
	virtual void setFocus();

private:
	QLineEdit* lineEdit;
};

#endif // TESTPAGE_H
