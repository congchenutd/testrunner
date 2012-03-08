#ifndef ANSWERAREA_H
#define ANSWERAREA_H

#include <QWidget>
#include <QVariant>
#include <QList>

class QVBoxLayout;
class AnswerArea : public QWidget
{
	Q_OBJECT
public:
	AnswerArea(QWidget* parent = 0);
	virtual ~AnswerArea() {}
	virtual QVariant getAnswer() const = 0;

public slots:
	virtual void showEvent(QShowEvent*);
	virtual bool validate() const;
	virtual void setFocus() {}

signals:
	void validated(bool) const;

protected:
	QVBoxLayout* vLayout;
};

class DefaultAnswerArea : public AnswerArea
{
public:
	DefaultAnswerArea(QWidget* parent = 0) : AnswerArea(parent){}
	virtual QVariant getAnswer() const { return QString(); }

protected:
	virtual bool validate() const { return true; }
};

class QRadioButton;
class SingleChoiceArea : public AnswerArea
{
public:
	SingleChoiceArea(QWidget* parent = 0) : AnswerArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QRadioButton*> radioButtons;
};

class QCheckBox;
class MultipleChoiceArea : public AnswerArea
{
public:
	MultipleChoiceArea(QWidget* parent = 0) : AnswerArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class QSpinBox;
class IntegerArea : public AnswerArea
{
public:
	IntegerArea(QWidget* parent = 0);
	void setValueRange(int min, int max);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QSpinBox* spinBox;
};

class QLineEdit;
class BlankFillingArea : public AnswerArea
{
public:
	BlankFillingArea(QWidget* parent = 0) : AnswerArea(parent) {}
	void addBlank(const QString& name);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QLineEdit*> lineEdits;
};

#endif // ANSWERAREA_H
