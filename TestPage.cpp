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

TestPage::TestPage(const QString& title, const QString& text, bool skip, bool name)
{
    layout = new QVBoxLayout(this);

	leTitle = new QLabel;
	leText  = new QLabel;
	leTitle->setWordWrap(true);
	leText ->setWordWrap(true);

    layout->addWidget(leTitle);
    layout->addWidget(leText);
    layout->addStretch();

	setTitle(title);
	setText (text);

	setFont(globalFont);
	leTitle->setFont(titleFont);
	leText ->setFont(textFont);

	maySkip = skip;
	isName  = name;
	answerArea = new DefaultAnswerArea;
}

QString TestPage::toString() const {
	return leTitle->text()        + "\t" +
		   getAnswer().toString() + "\t" +
		   QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
}

void TestPage::setAnswerArea(AnswerArea* answer)
{
    if(answer == 0)
        return;
    delete answerArea;
    answerArea = answer;
    layout->insertWidget(2, answerArea);
    connect(answerArea, SIGNAL(validated(AnswerStatus)), this, SIGNAL(statusChanged(AnswerStatus)));
}

void TestPage::validate() {
	emit statusChanged(maySkip ? IGNORED : answerArea->validate());
}

QVariant TestPage::getAnswer() const {
	return answerArea->getAnswer();
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
