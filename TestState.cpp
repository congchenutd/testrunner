#include "TestState.h"
#include "MainWindow.h"

TestState* TestState::initState     = 0;
TestState* TestState::loadedState   = 0;
TestState* TestState::answeredState = 0;
TestState* TestState::finishedState = 0;
TestState* TestState::nullState     = 0;

TestState* TestState::init()
{
	initState     = new InitState;
	loadedState   = new LoadedState;
	answeredState = new AnsweredState;
	finishedState = new FinishedState;
	nullState     = new NullState;
	return initState;
}
