#include "TestPage.h"
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

TestPage::TestPage(const QString& ttl, const QString& tx, bool skip, bool timeIt, bool name)
{
	setLayout(new QVBoxLayout);
	layout()->setSizeConstraint(QLayout::SetFixedSize);

	layout()->addWidget(&leTitle);
	layout()->addWidget(&leText);
	leTitle.setText(ttl);
	leText .setText(tx);
	leTitle.setHidden(ttl.isEmpty());   // empty title and text are hidden
	leText .setHidden(tx .isEmpty());

	setFont(globalFont);
	leTitle.setFont(titleFont);
	leText .setFont(textFont);

	maySkip = skip;
	isName  = name;

	elapsed = 0;
	if(timeIt)
	{
		QTimer* timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
		timer->start(1000);
	}
}

void TestPage::setFocus() {
	QWidget::setFocus();
}

QString TestPage::toString() const
{
	QString result = leTitle.text() + "\t" + getAnswer().toString();
	if(elapsed > 0)
		result += "\t" + tr("%1 seconds").arg(elapsed);
	return result;
}

// all subclasses should return QVariant() if the result is invalid
bool TestPage::validate() const {
	return accept(maySkip ? true : getAnswer().isValid());
}

bool TestPage::accept(bool ok) const
{
	emit valid(ok);
	return ok;
}

void TestPage::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	setFocus();   // automatically grab focus
}

void TestPage::onTimer() {
	elapsed ++;
}

void TestPage::setTitle(const QString& title) {
	leTitle.setText(title);
}

void TestPage::setText(const QString& text) {
	leText.setText(text);
}

void TestPage::setSkippable(bool skip) {
	maySkip = skip;
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

void TestPage::setIsName(bool name) {
	isName = name;
}

void TestPage::setDuration(const QTime& start, const QTime& end)
{
	startTime = start;
	endTime   = end;
}


/////////////////////////////////////////////////////////////////////////////////
bool TextPage::validate() const {
	return accept(true);   // nothing to validate
}


///////////////////////////////////////////////////////////////////////////////
void SingleChoicePage::addChoice(const QString& choice)
{
	QRadioButton* radio = new QRadioButton(choice, this);
	layout()->addWidget(radio);
	radioButtons << radio;
	connect(radio, SIGNAL(clicked()), this, SLOT(validate()));   // auto validate
}

QVariant SingleChoicePage::getAnswer() const
{
	foreach(QRadioButton* radio, radioButtons)
		if(radio->isChecked())
			return radio->text();
	return QVariant();
}

// focus on the first choice
void SingleChoicePage::setFocus() {
	if(!radioButtons.isEmpty())
		radioButtons.front()->setFocus();
}


////////////////////////////////////////////////////////////////////////////
void MultipleChoicePage::addChoice(const QString &choice)
{
	QCheckBox* checkBox = new QCheckBox(choice, this);
	layout()->addWidget(checkBox);
	checkBoxes << checkBox;
	connect(checkBox, SIGNAL(clicked()), this, SLOT(validate()));  // auto validate
}

QVariant MultipleChoicePage::getAnswer() const
{
	QStringList result;
	foreach(QCheckBox* checkBox, checkBoxes)
		if(checkBox->isChecked())
			result << checkBox->text();
	return result.isEmpty() ? QVariant() : result.join("\t");
}

// focus on the first choice
void MultipleChoicePage::setFocus() {
	if(!checkBoxes.isEmpty())
		checkBoxes.front()->setFocus();
}

/////////////////////////////////////////////////////////////////////////////////////////
IntegerPage::IntegerPage(int min, int max, const QString& title, const QString& text,
						 bool skip, bool timeIt)
	: TestPage(title, text, skip, timeIt)
{
	spinBox = new QSpinBox(this);
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	layout()->addWidget(spinBox);
}

QVariant IntegerPage::getAnswer() const {
	return spinBox->value();
}

void IntegerPage::setFocus()
{
	spinBox->setFocus();
	spinBox->selectAll();
}

/////////////////////////////////////////////////////////////////////////////////////////
BlankFillingPage::BlankFillingPage(const QString &title, const QString &text,
								   bool skip, bool timeIt, bool name)
	: TestPage(title, text, skip, timeIt, name)
{
	lineEdit = new QLineEdit(this);
	layout()->addWidget(lineEdit);
	connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(validate())); // auto validate
}

QVariant BlankFillingPage::getAnswer() const {
	return lineEdit->text().isEmpty() ? QVariant() : lineEdit->text();
}

void BlankFillingPage::setFocus() {
	lineEdit->setFocus();
}


