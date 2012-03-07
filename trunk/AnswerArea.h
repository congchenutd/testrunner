#ifndef ANSWERAREA_H
#define ANSWERAREA_H

#include <QWidget>
#include <QVariant>
#include <QList>

class QVBoxLayout;
class IAnswerArea : public QWidget
{
	Q_OBJECT
public:
	IAnswerArea(QWidget* parent = 0);
	virtual ~IAnswerArea() {}
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

class DefaultAnswerArea : public IAnswerArea
{
public:
	DefaultAnswerArea(QWidget* parent = 0) : IAnswerArea(parent){}
	virtual QVariant getAnswer() const { return QString(); }

protected:
	virtual bool validate() const { return true; }
};

class QRadioButton;
class SingleChoiceArea : public IAnswerArea
{
public:
	SingleChoiceArea(QWidget* parent = 0) : IAnswerArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QRadioButton*> radioButtons;
};

class QCheckBox;
class MultipleChoiceArea : public IAnswerArea
{
public:
	MultipleChoiceArea(QWidget* parent = 0) : IAnswerArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class QSpinBox;
class IntegerArea : public IAnswerArea
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
class BlankFillingArea : public IAnswerArea
{
public:
	BlankFillingArea(QWidget* parent = 0) : IAnswerArea(parent) {}
	void addBlank(const QString& name);

	virtual QVariant getAnswer() const;
	virtual void     setFocus();

private:
	QList<QLineEdit*> lineEdits;
};

#endif // ANSWERAREA_H
