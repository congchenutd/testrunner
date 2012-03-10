#ifndef TESTSTATE_H
#define TESTSTATE_H

class TestState
{
public:
	static TestState* init();

	TestState* gotoEnd() { return finishedState; }

	virtual TestState* gotoNext() = 0;
	virtual bool isQuitSafe()    const = 0;
	virtual bool hasAnswered()   const = 0;
	virtual bool isLoadEnabled() const = 0;
	virtual bool isNextEnabled() const = 0;
	virtual bool isQuitEnabled() const = 0;

protected:
	static TestState* initState;
	static TestState* loadedState;
	static TestState* answeredState;
	static TestState* finishedState;
	static TestState* nullState;
};

class InitState : public TestState
{
public:
	virtual TestState* gotoNext() { return loadedState; }
	virtual bool isQuitSafe()    const { return true;  }
	virtual bool hasAnswered()   const { return false; }
	virtual bool isLoadEnabled() const { return true;  }
	virtual bool isNextEnabled() const { return false; }
	virtual bool isQuitEnabled() const { return true;  }
};

class LoadedState : public TestState
{
public:
	virtual TestState* gotoNext() { return answeredState; }
	virtual bool isQuitSafe()    const { return false; }
	virtual bool hasAnswered()   const { return false; }
	virtual bool isLoadEnabled() const { return false; }
	virtual bool isNextEnabled() const { return true;  }
	virtual bool isQuitEnabled() const { return false; }
};

class AnsweredState : public TestState
{
public:
	virtual TestState* gotoNext() { return answeredState; }
	virtual bool isQuitSafe()    const { return false; }
	virtual bool hasAnswered()   const { return true;  }
	virtual bool isLoadEnabled() const { return false; }
	virtual bool isNextEnabled() const { return true;  }
	virtual bool isQuitEnabled() const { return false; }
};

class FinishedState : public TestState
{
public:
	virtual TestState* gotoNext() { return 0; }
	virtual bool isQuitSafe()    const { return true;  }
	virtual bool hasAnswered()   const { return true;  }
	virtual bool isLoadEnabled() const { return false; }
	virtual bool isNextEnabled() const { return false; }
	virtual bool isQuitEnabled() const { return true;  }
};

class NullState : public TestState
{
public:
	virtual TestState* gotoNext() { return nullState; }
	virtual bool isQuitSafe()    const { return true;  }
	virtual bool hasAnswered()   const { return false; }
	virtual bool isLoadEnabled() const { return true;  }
	virtual bool isNextEnabled() const { return true;  }
	virtual bool isQuitEnabled() const { return true;  }
};

#endif // TESTSTATE_H
