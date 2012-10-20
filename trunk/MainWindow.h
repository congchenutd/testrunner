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
    bool init(const QString& fileName = QString());  // init and load if fileName is nonempty

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void onLoad();                              // open a test file
    void onNext();                              // next clicked
    void onAnswered(AnswerStatus);              // answer changed
    void onTitleChanged(const QString& title);  // a new section

private:
    bool openTestFile(const QString& fileName); // read the XML
    void setPage(TestPage* page);               // change page
	void saveCurrentPage();
    void updateButtons();                       // enable/disable buttons based on state
	void gotoNextState();

    QString makeTempFileName() const;           // result is saved to the temp file first
    QString makeResultFileName() const;         // generate based on the user name

private:
	Ui::MainWindow ui;

	QFile       xmlFile;        // for input
	TestPage*   currentPage;

	QString     userName;       // for output
    QFile       tempFile;       // result is saved to the temp file first
	QTextStream os;

	TestLoader* loader;
    TestState*  state;          // controls button status, quit, and save
};

#endif // MAINWINDOW_H
