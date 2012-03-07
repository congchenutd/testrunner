#ifndef ANSWERAREA_H
#define ANSWERAREA_H

#include <QWidget>
#include <QVariant>
#include <QList>

class IAnswerArea : public QWidget
{
public:
	IAnswerArea(QWidget* parent = 0);
	virtual ~IAnswerArea() {}
	virtual QVariant getAnswer() const = 0;

public slots:
	virtual bool validate() const = 0;
};

class QVBoxLayout;
class DefaultArea : public IAnswerArea
{
public:
	DefaultArea(QWidget* parent = 0);
	virtual QVariant getAnswer() const { return QString(); }
	virtual bool     validate()  const { return !getAnswer().isNull(); }

protected:
	virtual void showEvent(QShowEvent*);
	virtual void setFocus() {}

protected:
	QVBoxLayout* vLayout;
};

class QRadioButton;
class SingleChoiceArea : public DefaultArea
{
public:
	SingleChoiceArea(QWidget* parent = 0) : DefaultArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QRadioButton*> radioButtons;
};

class QCheckBox;
class MultipleChoiceArea : public DefaultArea
{
public:
	MultipleChoiceArea(QWidget* parent = 0) : DefaultArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class QSpinBox;
class IntegerArea : public DefaultArea
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
class BlankFillingArea : public DefaultArea
{
public:
	BlankFillingArea(QWidget* parent = 0) : DefaultArea(parent) {}
	void addBlank(const QString& name);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QLineEdit*> lineEdits;
};

#endif // ANSWERAREA_H
