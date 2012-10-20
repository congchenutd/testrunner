#include "TestPage.h"
#include "AnswerArea.h"
#include <QDateTime>

QFont TestPage::globalFont;
QFont TestPage::titleFont;
QFont TestPage::textFont;

TestPage* TestPage::getNullPage() {
    return new TestPage(QString(), QString());
}

TestPage::TestPage(const QString& title, const QString& text, bool skip, bool name)
    : leTitle(this),
      leText(this),
      maySkip(skip),
      isName(name),
      layout(this),
      answerArea(new DefaultAnswerArea)
{
    leTitle.setWordWrap(true);
    leText .setWordWrap(true);

    layout.addWidget(&leTitle);
    layout.addWidget(&leText);
    layout.addStretch();

	setTitle(title);
	setText (text);

	setFont(globalFont);
    leTitle.setFont(titleFont);
    leText .setFont(textFont);
}

QString TestPage::toString() const
{
    if(leTitle.text().isEmpty())    // null page
        return QString();
    return leTitle.text()         + "\t" +
           getAnswer().toString() + "\t" +
           QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss") + "\r\n";
}

void TestPage::setAnswerArea(AnswerArea* answer)
{
    if(answer == 0)
        return;
    delete answerArea;
    answerArea = answer;
    layout.insertWidget(2, answerArea);
    connect(answerArea, SIGNAL(validated(AnswerStatus)), this, SIGNAL(validated(AnswerStatus)));
}

void TestPage::validate()
{
    AnswerStatus answerStatus = answerArea->validate();  // only VALID or INVALID
    emit validated((answerStatus == VALID) ? VALID
                                           : (maySkip ? IGNORED
                                                      : INVALID));
}

QVariant TestPage::getAnswer() const {
	return answerArea->getAnswer();
}

void TestPage::setTitle(const QString& title)
{
    leTitle.setText(title);
    leTitle.setHidden(title.isEmpty());   // empty title is hidden
}

void TestPage::setText(const QString& text)
{
    leText.setText(text);
    leText.setHidden(text.isEmpty());     // empty text is hidden
}
