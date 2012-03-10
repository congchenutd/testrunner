#include "MainWindow.h"
#include "TestPage.h"
#include "TestState.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), userName("UnkownParticipant")
{
	ui.setupUi(this);

	currentPage = 0;
	loader = new TestLoader(this);

	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

bool MainWindow::init(const QString& fileName)
{
	gotoInitState();

	// open the temp file
	tempFile.setFileName(makeTempFileName());
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

void MainWindow::updateButtons()
{
	ui.actionLoad->setVisible(state->isLoadEnabled());
	ui.actionNext->setVisible(state->isNextEnabled());
	ui.actionQuit->setVisible(state->isQuitEnabled());
}

void MainWindow::gotoInitState()
{
	state = TestState::init();
	updateButtons();
}

void MainWindow::gotoNextState()
{
	state = state->gotoNext();
	updateButtons();
}

void MainWindow::gotoEndState()
{
	state = state->gotoEnd();
	updateButtons();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	// need confirmation if it's not safe to quit
	if(!state->isQuitSafe()) {
		if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure to abort the test?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
		{
			saveCurrentPage();        // don't forget current page
			os << "\r\nUnfinished";   // Mark "Unfinished"
		}
		else
			return event->ignore();   // quitting canceled
	}

	if(state->hasAnswered())                              // result is not empty
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
		gotoNextState();
		setPage(loader->loadNext());          // style and intro
	}
}

void MainWindow::onNext()
{
	setPage(loader->loadNext());          // load a section or question

	if(loader->atEnd())                   // end of xml
		gotoEndState();
}

void MainWindow::onTestStatus(AnswerStatus status)
{
	ui.actionNext->setEnabled(status != INVALID);  // IGNORED or VALID
	if(status == VALID)
		gotoNextState();
}

void MainWindow::setPage(TestPage* page)
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

	connect(currentPage, SIGNAL(statusChanged(AnswerStatus)), this, SLOT(onTestStatus(AnswerStatus)));
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
	return QFileInfo(xmlFile.fileName()).baseName() + "_" + tr("TestResult")
			+ QDateTime::currentDateTime().toString("@yyyy-MM-dd-hh-mm-ss")
			+ ".tmp";
}

QString MainWindow::makeResultFileName() const {
	return QFileInfo(xmlFile.fileName()).baseName() + "_" + userName
			+ QDateTime::currentDateTime().toString("@yyyy-MM-dd-hh-mm-ss")
			+ ".txt";
}
