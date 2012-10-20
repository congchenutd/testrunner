#ifndef TESTSTATE_H
#define TESTSTATE_H

// Determine whether the actions are enabled and whether the app can quit
// Each state knows what's the next state
// MainWindow determines when to jump to the next state
class TestState
{
public:
    static TestState* getUninitState();
    static TestState* getInitState();
    static TestState* getLoadedState();
    static TestState* getAnsweredState();
    static TestState* getFinishedState();

    virtual TestState* next() = 0;
    virtual bool isQuitSafe()    const = 0;
    virtual bool hasAnswered()   const = 0;
    virtual bool isLoadEnabled() const = 0;
    virtual bool isNextEnabled() const = 0;
    virtual bool isQuitEnabled() const = 0;
};

// before init
class UninitState : public TestState
{
public:
    TestState* next() { return getInitState(); }
    bool isQuitSafe()    const { return true;  }
    bool hasAnswered()   const { return false; }
    bool isLoadEnabled() const { return true;  }
    bool isNextEnabled() const { return true;  }
    bool isQuitEnabled() const { return true;  }
};

// after the app is up
class InitState : public TestState
{
public:
    TestState* next() { return getLoadedState(); }
    bool isQuitSafe()    const { return true;  }
    bool hasAnswered()   const { return false; }
    bool isLoadEnabled() const { return true;  }
    bool isNextEnabled() const { return false; }
    bool isQuitEnabled() const { return true;  }
};

// after the xml is loaded
class LoadedState : public TestState
{
public:
    TestState* next() { return getAnsweredState(); }
    bool isQuitSafe()    const { return false; }
    bool hasAnswered()   const { return false; }
    bool isLoadEnabled() const { return false; }
    bool isNextEnabled() const { return true;  }
    bool isQuitEnabled() const { return false; }
};

// current question is answered
class AnsweredState : public TestState
{
public:
    TestState* next() { return getAnsweredState(); }
    bool isQuitSafe()    const { return false; }
    bool hasAnswered()   const { return true;  }
    bool isLoadEnabled() const { return false; }
    bool isNextEnabled() const { return true;  }
    bool isQuitEnabled() const { return false; }
};

// all questions are answered
class FinishedState : public TestState
{
public:
    TestState* next() { return getFinishedState(); }
    bool isQuitSafe()    const { return true;  }
    bool hasAnswered()   const { return true;  }
    bool isLoadEnabled() const { return false; }
    bool isNextEnabled() const { return false; }
    bool isQuitEnabled() const { return true;  }
};


#endif // TESTSTATE_H
