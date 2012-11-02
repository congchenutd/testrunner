#include "MainWindow.h"
#include "TestPage.h"
#include "TestState.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentPage(TestPage::getNullPage()),
    userName("UnkownParticipant"),
    loader(new TestLoader(this)),
    state(TestState::getUninitState())
{
	ui.setupUi(this);

	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(loader, SIGNAL(titleChanged(QString)), this, SLOT(onTitleChanged(QString)));
}

bool MainWindow::init(const QString& fileName)
{
    gotoNextState();

	// open the temp file
	tempFile.setFileName(makeTempFileName());
	if(!tempFile.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	os.setDevice(&tempFile);

    // load test file
    if(!fileName.isEmpty())
        return openTestFile(fileName);
	return true;
}

void MainWindow::onTitleChanged(const QString& title) {
	setWindowTitle(tr("Test Runner - ") + title);
}

void MainWindow::updateButtons()
{
	ui.actionLoad->setVisible(state->isLoadEnabled());
	ui.actionNext->setVisible(state->isNextEnabled());
	ui.actionQuit->setVisible(state->isQuitEnabled());
}

void MainWindow::gotoNextState()
{
    state = loader->atEnd() ? state->getFinishedState() : state->next();
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
            os << "\r\nUnfinished";   // mark "Unfinished"
		}
		else
			return event->ignore();   // quitting canceled
	}

    if(state->hasAnswered())                  // result is not empty
		tempFile.copy(makeResultFileName());  // copy result
	tempFile.remove();                        // remove temp
	return QMainWindow::closeEvent(event);

}

void MainWindow::onLoad()
{
	QString fileName = QFileDialog::getOpenFileName(
						   this, tr("Open test configuration"),
						   QDir::currentPath(), tr("Tests (*.xml);;All files (*.*)"));
    activateWindow();             // re-gain focus after the file dialog

    if(!fileName.isEmpty())
        openTestFile(fileName);
}

bool MainWindow::openTestFile(const QString& fileName)
{
    if(fileName.isEmpty())
        return false;

	// open file
	xmlFile.setFileName(fileName);
	if(!xmlFile.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Cannot open the file"));
		return false;
	}

    // is content correct?
	if(loader->openXMLFile(xmlFile))
	{
        // stay on top
        setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
        show();

        onNext();      // load style and intro
		return true;
	}
	return false;
}

void MainWindow::onNext()
{
	setPage(loader->loadNext());          // load a section or question
    gotoNextState();
}

void MainWindow::onAnswered(AnswerStatus status)
{
	ui.actionNext->setEnabled(status != INVALID);  // IGNORED or VALID
    if(status == VALID)                            // IGNORED doesn't change state
		gotoNextState();
}

void MainWindow::setPage(TestPage* page)
{
	if(page == 0)
		return;

	// save and remove the old page
    saveCurrentPage();
    TestPage* tobeDeleted = currentPage;

	// set new page
	currentPage = page;
	setCentralWidget(page);
    delete tobeDeleted;
    connect(page, SIGNAL(validated(AnswerStatus)), this, SLOT(onAnswered(AnswerStatus)));
    currentPage->validate();    // init the "Next" button
}

void MainWindow::saveCurrentPage()
{
    os << currentPage->toString();
    os.flush();
    if(currentPage->isNamePage() && !currentPage->getAnswer().isNull())
        userName = currentPage->getAnswer().toString();   // save user name
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
