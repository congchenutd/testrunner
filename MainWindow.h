#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>

class TestPage;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	bool init(const QString& fileName = QString());

protected:
	virtual void closeEvent(QCloseEvent*);

private slots:
	void onLoad();
	void onNext();

private:
	void setTestFile(const QString& fileName);
	QFont loadFont();                       // <-- load test info from xml
	void loadStyle();
	void loadIntro(const QString& title);
	void loadSection();
	void loadQuestion();                    // -->
	void finish();                          // show end page
	void setPage(TestPage* page);           // change page
	void saveCurrentPage();

	QString makeTempFileName() const;       // result is saved to the temp file first
	QString makeResultFileName() const;     // generate based on the user name

private:
	Ui::MainWindow ui;

	QFile            xmlFile;        // for input
	QXmlStreamReader xml;
	TestPage*        currentPage;

	QString     userName;            // for output
	QString     tempFileName;
	QFile       tempFile;
	QTextStream os;

	bool quitSafe;    // it's saft to quit before loading and after finishing
	bool answered;    // is any questions answered. save the result if yes
};

#endif // MAINWINDOW_H
