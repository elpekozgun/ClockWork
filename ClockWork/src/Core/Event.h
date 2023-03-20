#pragma once

#include <unordered_set>
#include <functional>
#include <memory>
#include <iostream>

#include "Core/Core.h"

using namespace std;

class IDelegate
{
public:
	virtual void operator()() = 0;
	virtual bool operator==(IDelegate* other) = 0;
};

template<typename T>
class Delegate : public IDelegate
{
public:
	Delegate() = default;
	virtual ~Delegate() = default;

	void Set(T* instance, void (T::* action)())
	{
		_instance = instance;
		_action = action;
	}

	void operator()() override
	{
		(_instance->*_action)();
	}

	inline bool operator==(IDelegate* other) override
	{
		Delegate* otherDelegate = dynamic_cast<Delegate*>(other);
		if (otherDelegate == nullptr)
			return false;
		return (this->_action == otherDelegate->_action) &&
			(this->_instance == otherDelegate->_instance);
	}

private:
	void (T::* _action)();
	T* _instance;
};



class CW_API Event
{
public:
	Event() = default;

	void operator+=(IDelegate* delegate)
	{
		_invocationList.insert(delegate);
	}

	void operator-=(IDelegate* delegate)
	{
		_invocationList.erase(delegate);
	}

	void Invoke() const;

	IDelegate* operator=(IDelegate* delegate) = delete;

private:
	typedef std::unordered_set<IDelegate*> Delegates;
	Delegates _invocationList;
};


