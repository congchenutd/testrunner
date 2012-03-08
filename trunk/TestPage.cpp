#include "TestPage.h"
#include "AnswerArea.h"
#include <QVBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

QFont TestPage::globalFont;
QFont TestPage::titleFont;
QFont TestPage::textFont;

TestPage::TestPage(const QString& title, const QString& text, bool skip, bool timer, bool name)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSizeConstraint(QLayout::SetFixedSize);


	leTitle = new QLabel;
	leText  = new QLabel;
	leTitle->setWordWrap(true);
	leText ->setWordWrap(true);
	layout->addWidget(leTitle);
	layout->addWidget(leText);
	setTitle(title);
	setText (text);

	setFont(globalFont);
	leTitle->setFont(titleFont);
	leText ->setFont(textFont);

	maySkip = skip;
	isName  = name;
	setTimerEnabled(timer);
	answerArea = new DefaultAnswerArea;
}

QString TestPage::toString() const
{
	QString result = leTitle->text() + "\t" + getAnswer().toString();
	if(elapsed > 0)
		result += "\t" + tr("%1 seconds").arg(elapsed);
	return result;
}

void TestPage::setAnswerArea(AnswerArea* answer)
{
	if(answer == 0)
		return;
	delete answerArea;
	answerArea = answer;
	layout()->addWidget(answerArea);
	layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Preferred, QSizePolicy::Expanding));
	connect(answerArea, SIGNAL(validated(bool)), this, SIGNAL(valid(bool)));
}

bool TestPage::validate() const {
	return maySkip ? accept(true) : accept(answerArea->validate());
}

bool TestPage::accept(bool ok) const
{
	emit valid(ok);
	return ok;
}

QVariant TestPage::getAnswer() const {
	return answerArea->getAnswer();
}

void TestPage::onTimer() {
	elapsed ++;
}

void TestPage::setTitle(const QString& title)
{
	leTitle->setText(title);
	leTitle->setHidden(title.isEmpty());   // empty title is hidden
}

void TestPage::setText(const QString& text)
{
	leText->setText(text);
	leText->setHidden(text.isEmpty());     // empty text is hidden
}

void TestPage::setTimerEnabled(bool enable)
{
	elapsed = 0;
	if(enable)
	{
		QTimer* timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
		timer->start(1000);
	}
}
