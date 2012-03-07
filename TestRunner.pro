#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T13:52:47
#
#-------------------------------------------------

QT       += core gui

TARGET = TestRunner
TEMPLATE = app


SOURCES +=\
	MainWindow.cpp \
	TestPage.cpp \
	Main.cpp \
    TestLoader.cpp \
    AnswerAreaFactory.cpp \
    AnswerArea.cpp

HEADERS  += \
	MainWindow.h \
	TestPage.h \
    TestLoader.h \
    AnswerAreaFactory.h \
    AnswerArea.h

FORMS    += \
	MainWindow.ui

RESOURCES += \
	TestRunner.qrc

ICON = Images/Test.icns
RC_FILE = TestRunner.rc
