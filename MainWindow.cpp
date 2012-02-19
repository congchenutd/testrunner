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

void MainWindow::setTestFile(const QString& fileName)
{
	xmlFile.setFileName(fileName);
	if(!xmlFile.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Cannot open the file"));
		return;
	}

	xml.setDevice(&xmlFile);
	if(xml.readNextStartElement() && xml.name() == "test")
	{
		quitSafe = false;
		ui.actionLoad->setVisible(false);
		ui.actionQuit->setVisible(false);
		ui.actionNext->setVisible(true);

		if(xml.readNextStartElement() && xml.name() == "style")
			loadStyle();
		if(xml.readNextStartElement() && xml.name() == "intro")
			loadIntro(tr("Introduction"));
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	// confirm if it's not safe to quit
	if(!quitSafe) {
		if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure to abort the test?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
		{
			saveCurrentPage();        // don't forget current page
			os << "\r\nUnfinished";
		}
		else
			return event->ignore();   // quitting canceled
	}

	if(answered)                              // result is not empty
		tempFile.copy(makeResultFileName());  // copy result
	tempFile.remove();                        // remove temp
	return QMainWindow::closeEvent(event);

}

void MainWindow::onLoad()
{
	QString fileName = QFileDialog::getOpenFileName(
						   this, tr("Open test configuration"),
						   QDir::currentPath(), tr("Tests (*.xml);;All files (*.*)"));

	activateWindow();             // recapture focus after the file dialog
	if(!fileName.isEmpty())
		setTestFile(fileName);
}

QFont MainWindow::loadFont()
{
	QFont result = font();        // use system font by default
	if(xml.readNextStartElement() && xml.name() == "fontfamily")
		result.setFamily(xml.readElementText());
	if(xml.readNextStartElement() && xml.name() == "fontweight") {
		if(xml.readElementText() == "normal")
			result.setWeight(QFont::Normal);
		else if(xml.readElementText() == "bold")
			result.setWeight(QFont::Bold);
	}
	if(xml.readNextStartElement() && xml.name() == "fontsize")
		result.setPointSize(xml.readElementText().toInt());
	return result;
}

void MainWindow::loadStyle()
{
	if(!xml.isStartElement() || xml.name() != "style")
		return;

	while(!(xml.isEndElement() && xml.name() == "style")) {
		if(xml.readNextStartElement())
		{
			if(xml.name() == "global")
				TestPage::setGlobalFont(loadFont());
			else if(xml.name() == "title")
			{
				QString n = xml.name().toString();
				TestPage::setTitleFont(loadFont());
			}
			else if(xml.name() == "text")
				TestPage::setTextFont(loadFont());
		}
	}
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
		SingleChoicePage* page = new SingleChoicePage(title, content, maySkip, timeIt);
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
		if(currentPage->containsName())     // save user name
			userName = currentPage->getAnswer().toString();
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
			+ ".txt";
}

QString MainWindow::makeResultFileName() const {
	return userName
			+ QDateTime::currentDateTime().toString("_yyyy-MM-dd-hh-mm-ss")
			+ ".txt";
}
