#include "AnswerArea.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

IAnswerArea::IAnswerArea(QWidget* parent) : QWidget(parent)
{
	vLayout = new QVBoxLayout(this);
	setLayout(vLayout);
}

bool IAnswerArea::validate() const
{
	bool result = !getAnswer().isNull();
	emit validated(result);
	return result;
}

void IAnswerArea::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	setFocus();   // automatically grab focus
}

/////////////////////////////////////////////////////////////////////////////////////
void SingleChoiceArea::addChoice(const QString& choice)
{
	QRadioButton* radio = new QRadioButton(choice, this);
	vLayout->addWidget(radio);
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

void SingleChoiceArea::setFocus() {
	if(!radioButtons.isEmpty())   // focus on the first choice
		radioButtons.front()->setFocus();
}

/////////////////////////////////////////////////////////////
void MultipleChoiceArea::addChoice(const QString &choice)
{
	QCheckBox* checkBox = new QCheckBox(choice, this);
	vLayout->addWidget(checkBox);
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

void MultipleChoiceArea::setFocus() {
	if(!checkBoxes.isEmpty())   // focus on the first choice
		checkBoxes.front()->setFocus();
}

//////////////////////////////////////////////////////////////
IntegerArea::IntegerArea(QWidget* parent) : IAnswerArea(parent)
{
	spinBox = new QSpinBox(this);
	vLayout->addWidget(spinBox);
}

void IntegerArea::setValueRange(int min, int max)
{
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
}

QVariant IntegerArea::getAnswer() const {
	return spinBox->value();
}

void IntegerArea::setFocus()
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
	vLayout->addLayout(hLayout);
}

QVariant BlankFillingArea::getAnswer() const
{
	bool allEmpty = true;
	QStringList result;
	foreach(QLineEdit* edit, lineEdits)
	{
		QString text = edit->text();
		if(!text.isEmpty())
			allEmpty = false;
		result << text;
	}
	return allEmpty ? QVariant() : result.join("\t");
}

void BlankFillingArea::setFocus() {
	if(!lineEdits.isEmpty())
		lineEdits.front()->setFocus();
}

