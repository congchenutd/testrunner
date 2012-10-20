#include "TestState.h"

TestState *TestState::getUninitState()
{
    static UninitState uninitState;
    return &uninitState;
}

TestState* TestState::getInitState()
{
    static InitState initState;
    return &initState;
}

TestState* TestState::getLoadedState()
{
    static LoadedState loadedState;
    return &loadedState;
}

TestState* TestState::getAnsweredState()
{
    static AnsweredState answeredState;
    return &answeredState;
}

TestState* TestState::getFinishedState()
{
    static FinishedState finishedState;
    return &finishedState;
}
