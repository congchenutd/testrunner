#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TestLoader.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

class TestPage;
class TestLoader;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	bool init(const QString& fileName = QString());

	void setTitle(const QString& title);               // <- called by loader
	void setAnswered(bool an) { answered = an; }       // ->

protected:
	virtual void closeEvent(QCloseEvent*);

private slots:
	void onLoad();
	void onNext();

private:
	void setTestFile(const QString& fileName);
	void setPage(TestPage* page);           // change page
	void saveCurrentPage();

	QString makeTempFileName() const;       // result is saved to the temp file first
	QString makeResultFileName() const;     // generate based on the user name

private:
	Ui::MainWindow ui;

	QFile       xmlFile;        // for input
	TestPage*   currentPage;

	QString     userName;       // for output
	QString     tempFileName;
	QFile       tempFile;
	QTextStream os;

	bool quitSafe;    // it's saft to quit before loading and after finishing
	bool answered;    // is any questions answered. save the result if yes

	TestLoader* loader;
};

#endif // MAINWINDOW_H
