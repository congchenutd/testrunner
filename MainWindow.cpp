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

	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

bool MainWindow::init(const QString& fileName)
{
	tempFileName = makeTempFileName();
	tempFile.setFileName(tempFileName);
	if(!tempFile.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	os.setDevice(&tempFile);

	if(!fileName.isEmpty())      // load test file
		setTestFile(fileName);
	return true;
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
	xml.setDevice(&xmlFile);
	if(xml.readNextStartElement() && xml.name() == "test")
	{
		quitSafe = false;
		ui.actionLoad->setVisible(false);    // change action status
		ui.actionQuit->setVisible(false);
		ui.actionNext->setVisible(true);
		loader.setXML(&xml);

		// style sheet
		if(xml.readNextStartElement() && xml.name() == "style")
			loader.loadStyle();

		// introduction of the test
		if(xml.readNextStartElement() && xml.name() == "intro")
			loadIntro(tr("Introduction"));
	}
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

void MainWindow::loadIntro(const QString& title)
{
	if(!xml.isStartElement() || xml.name() != "intro")
		return;

	QString introText = xml.readElementText();
	if(introText.isEmpty())
		return;

	setWindowTitle(title);
	setPage(new TextPage(title, introText));
}

void MainWindow::loadSection()
{
	if(!xml.isStartElement() || xml.name() != "section")
		return;

	QString title = xml.attributes().value("title").toString();

	if(xml.readNextStartElement()) {
		if(xml.name() == "intro")
			loadIntro(title);
		else if(xml.name() == "question")
			loadQuestion();
	}
}

void MainWindow::loadQuestion()
{
	if(!xml.isStartElement() || xml.name() != "question")
		return;

	QString title   = xml.attributes().value("title")  .toString();
	bool    maySkip = xml.attributes().value("mayskip").toString() == "true";
	bool    timeIt  = xml.attributes().value("timeit") .toString() == "true";
	bool    isName  = xml.attributes().value("isname") .toString() == "true";
	QString start   = xml.attributes().value("start")  .toString();
	QString end     = xml.attributes().value("end")    .toString();

	if(!xml.readNextStartElement() || xml.name() != "content")
		return;

	QString content = xml.readElementText();
	if(content.isEmpty())
		return;

	if(!xml.readNextStartElement() || xml.name() != "answer")
		return;

	if(!xml.readNextStartElement())
		return;

	answered = true;
	if(xml.name() == "integer")
	{
		int min = xml.attributes().value("min").toString().toInt();
		int max = xml.attributes().value("max").toString().toInt();
		setPage(new IntegerPage(min, max, title, content, maySkip, timeIt));
	}
	else if(xml.name() == "single")
	{
		SingleChoicePage* page   = new SingleChoicePage(title, content, maySkip, timeIt);
		while(xml.readNextStartElement() && xml.name() == "choice")
			page->addChoice(xml.readElementText());
		setPage(page);
	}
	else if(xml.name() == "multiple")
	{
		MultipleChoicePage* page = new MultipleChoicePage(title, content, maySkip, timeIt);
		while(xml.readNextStartElement() && xml.name() == "choice")
			page->addChoice(xml.readElementText());
		setPage(page);
	}
	else if(xml.name() == "blank") {
		setPage(new BlankFillingPage(title, content, maySkip, timeIt, isName));
	}
}

// show the finish page
void MainWindow::finish()
{
	quitSafe = true;
	ui.actionNext->setVisible(false);
	ui.actionQuit->setVisible(true);
	setPage(new TextPage(tr("Finished"),
						 tr("Thank you for your cooperation! You may quit the test now.")));
}

void MainWindow::setPage(TestPage *page)
{
	// save and remove the old page
	if(currentPage != 0)
	{
		saveCurrentPage();
		delete currentPage;
	}

	if(page == 0)
		return;

	// set new page
	currentPage = page;
	setCentralWidget(page);
	page->setFocus();

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

void MainWindow::onNext()
{
	// find a section or question
	while(!xml.atEnd())
		if(xml.readNextStartElement()) {
			if(xml.name() == "section")
				loadSection();
			else if(xml.name() == "question")
				loadQuestion();
			break;
		}

	// end of xml
	if(xml.atEnd())
		finish();
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
