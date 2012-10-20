#ifndef ANSWERAREA_H
#define ANSWERAREA_H

#include <QWidget>
#include <QVariant>
#include <QList>
#include <QVBoxLayout>

typedef enum {IGNORED, VALID, INVALID} AnswerStatus;

class AnswerArea : public QWidget
{
	Q_OBJECT
public:
	AnswerArea(QWidget* parent = 0);
	virtual ~AnswerArea() {}
	virtual QVariant getAnswer() const = 0;

public slots:
    virtual AnswerStatus validate() const;   // validate the answer

protected:
    virtual void showEvent(QShowEvent*);
    virtual void grabFocus() = 0;

signals:
	void validated(AnswerStatus) const;

protected:
    QVBoxLayout layout;
};

// An empty area
class DefaultAnswerArea : public AnswerArea
{
public:
    DefaultAnswerArea(QWidget* parent = 0) : AnswerArea(parent) {}
	virtual QVariant getAnswer() const { return QString(); }
    virtual AnswerStatus validate() const { return IGNORED; }

protected:
    virtual void grabFocus() {}
};

class QRadioButton;
class SingleChoiceArea : public AnswerArea
{
public:
	SingleChoiceArea(QWidget* parent = 0) : AnswerArea(parent) {}
	void addChoice(const QString& choice);

	virtual QVariant getAnswer() const;

protected:
    virtual void grabFocus();

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

protected:
    virtual void grabFocus();

private:
	QList<QCheckBox*> checkBoxes;
};

class QSpinBox;
class IntegerArea : public AnswerArea
{
public:
	IntegerArea(int min, int max, const QString& text, QWidget* parent = 0);

	virtual QVariant getAnswer() const;

protected:
    virtual void grabFocus();

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

protected:
    virtual void grabFocus();

private:
	QList<QLineEdit*> lineEdits;
};

#endif // ANSWERAREA_H
