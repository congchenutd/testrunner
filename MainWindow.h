#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TestLoader.h"
#include "AnswerArea.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

class TestState;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	bool init(const QString& fileName = QString());

	// called by loader
	void setTitle(const QString& title);

protected:
	virtual void closeEvent(QCloseEvent*);

private slots:
	void onLoad();
	void onNext();
	void onTestStatus(AnswerStatus);

private:
	void setTestFile(const QString& fileName);
	void setPage(TestPage* page);           // change page
	void saveCurrentPage();
	void updateButtons();
	void gotoInitState();
	void gotoNextState();
	void gotoEndState();

	QString makeTempFileName() const;       // result is saved to the temp file first
	QString makeResultFileName() const;     // generate based on the user name

private:
	Ui::MainWindow ui;

	QFile       xmlFile;        // for input
	TestPage*   currentPage;

	QString     userName;       // for output
	QFile       tempFile;
	QTextStream os;

	TestLoader* loader;
	TestState* state;
};

#endif // MAINWINDOW_H
