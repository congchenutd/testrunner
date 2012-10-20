#include "AnswerArea.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

AnswerArea::AnswerArea(QWidget* parent)
    : QWidget(parent), layout(this)
{
    setLayout(&layout);
}

AnswerStatus AnswerArea::validate() const
{
    bool valid = !getAnswer().isNull();
	AnswerStatus result = valid ? VALID : INVALID;
    emit validated(result);
    return result;
}

void AnswerArea::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
    grabFocus();   // automatically grab focus
}

/////////////////////////////////////////////////////////////////////////////////////
void SingleChoiceArea::addChoice(const QString& choice)
{
	QRadioButton* radio = new QRadioButton(choice, this);
    layout.addWidget(radio);
	radioButtons << radio;
	connect(radio, SIGNAL(clicked()), this, SLOT(validate()));   // auto validate
}

QVariant SingleChoiceArea::getAnswer() const
{
	foreach(QRadioButton* radio, radioButtons)
		if(radio->isChecked())
			return radio->text();
	return QVariant();
}

void SingleChoiceArea::grabFocus() {
	if(!radioButtons.isEmpty())   // focus on the first choice
		radioButtons.front()->setFocus();
}

/////////////////////////////////////////////////////////////
void MultipleChoiceArea::addChoice(const QString &choice)
{
	QCheckBox* checkBox = new QCheckBox(choice, this);
    layout.addWidget(checkBox);
	checkBoxes << checkBox;
	connect(checkBox, SIGNAL(clicked()), this, SLOT(validate()));  // auto validate
}

QVariant MultipleChoiceArea::getAnswer() const
{
	QStringList result;
	foreach(QCheckBox* checkBox, checkBoxes)
		if(checkBox->isChecked())
			result << checkBox->text();
	return result.isEmpty() ? QVariant() : result.join("\t");
}

void MultipleChoiceArea::grabFocus() {
	if(!checkBoxes.isEmpty())   // focus on the first choice
		checkBoxes.front()->setFocus();
}

//////////////////////////////////////////////////////////////
IntegerArea::IntegerArea(int min, int max, const QString& text, QWidget* parent) : AnswerArea(parent)
{
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->addWidget(new QLabel(text, this));
	spinBox = new QSpinBox(this);
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	hLayout->addWidget(spinBox);
    layout.addLayout(hLayout);
}

QVariant IntegerArea::getAnswer() const {
	return spinBox->value();
}

void IntegerArea::grabFocus()
{
	spinBox->setFocus();
	spinBox->selectAll();
}

////////////////////////////////////////////////////////////
void BlankFillingArea::addBlank(const QString& name)
{
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->addWidget(new QLabel(name, this));
	QLineEdit* lineEdit = new QLineEdit(this);
	hLayout->addWidget(lineEdit);
	connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(validate())); // auto validate

	lineEdits << lineEdit;
    layout.addLayout(hLayout);
}

QVariant BlankFillingArea::getAnswer() const
{
	QStringList result;
	foreach(QLineEdit* edit, lineEdits)
        if(!edit->text().simplified().isEmpty())
			result << edit->text();
	return result.isEmpty() ? QVariant() : result.join("\t");
}

void BlankFillingArea::grabFocus() {
	if(!lineEdits.isEmpty())
		lineEdits.front()->setFocus();
}

