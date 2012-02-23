#include "MainWindow.h"
#include "TestPage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), userName("UnkownParticipant")
{
	ui.setupUi(this);
	ui.actionNext->setVisible(false);

	quitSafe = true;
	answered = false;
	currentPage = 0;
	loader = new TestLoader(this);

	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

bool MainWindow::init(const QString& fileName)
{
	// open the temp file
	tempFileName = makeTempFileName();
	tempFile.setFileName(tempFileName);
	if(!tempFile.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	os.setDevice(&tempFile);

	if(!fileName.isEmpty())      // load test file
		setTestFile(fileName);
	return true;
}

void MainWindow::setTitle(const QString& title) {
	setWindowTitle(tr("Test Runner - ") + title);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	// need confirmation if it's not safe to quit
	if(!quitSafe) {
		if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure to abort the test?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
		{
			saveCurrentPage();        // don't forget current page
			os << "\r\nUnfinished";   // Mark "Unfinished"
		}
		else
			return event->ignore();   // quitting canceled
	}

	if(answered)                              // result is not empty
		tempFile.copy(makeResultFileName());  // copy result
	tempFile.remove();                        // remove temp
	return QMainWindow::closeEvent(event);

}

// open a file dialog for an XML file
void MainWindow::onLoad()
{
	QString fileName = QFileDialog::getOpenFileName(
						   this, tr("Open test configuration"),
						   QDir::currentPath(), tr("Tests (*.xml);;All files (*.*)"));

	activateWindow();             // re-gain focus after the file dialog
	if(!fileName.isEmpty())
		setTestFile(fileName);
}

// read the XML
void MainWindow::setTestFile(const QString& fileName)
{
	// open file
	xmlFile.setFileName(fileName);
	if(!xmlFile.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Cannot open the file"));
		return;
	}

	// content correct
	if(loader->openXMLFile(xmlFile))
	{
		quitSafe = false;
		ui.actionLoad->setVisible(false);     // change action status
		ui.actionQuit->setVisible(false);
		ui.actionNext->setVisible(true);
		setPage(loader->loadNext());          // style and intro
	}
}

void MainWindow::onNext()
{
	setPage(loader->loadNext());          // load a section or question

	if(loader->atEnd())                   // end of xml
	{
		quitSafe = true;
		ui.actionNext->setVisible(false);
		ui.actionQuit->setVisible(true);
		setPage(loader->loadEndPage());   // show the end page
	}
}

void MainWindow::setPage(TestPage *page)
{
	if(page == 0)
		return;

	// save and remove the old page
	if(currentPage != 0)
	{
		saveCurrentPage();
		delete currentPage;
	}

	// set new page
	currentPage = page;
	setCentralWidget(page);

	connect(page, SIGNAL(valid(bool)), ui.actionNext, SLOT(setEnabled(bool)));
	currentPage->validate();    // will enable/disable the "Next" button
}

void MainWindow::saveCurrentPage() {
	if(currentPage != 0)
	{
		os << currentPage->toString() << "\r\n";
		if(currentPage->isNamePage() && !currentPage->getAnswer().isNull())
			userName = currentPage->getAnswer().toString();   // save user name
	}
}

QString MainWindow::makeTempFileName() const {
	return tr("TestResult")
			+ QDateTime::currentDateTime().toString("_yyyy-MM-dd-hh-mm-ss")
			+ ".tmp";
}

QString MainWindow::makeResultFileName() const {
	return userName
			+ QDateTime::currentDateTime().toString("_yyyy-MM-dd-hh-mm-ss")
			+ ".txt";
}
