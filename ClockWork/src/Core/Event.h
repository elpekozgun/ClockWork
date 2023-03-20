#pragma once

#include <unordered_set>
#include <functional>
#include <memory>
#include <iostream>

#include "Core/Core.h"

using namespace std;


template<typename S>
class IDelegate
{
public:
	virtual void operator()(S param) = 0;
	virtual bool operator==(IDelegate<S>* other) = 0;
};

template<typename T, typename S>
class Delegate : public IDelegate<S>
{
public:
	Delegate() = default;
	virtual ~Delegate() = default;

	void Set(T* instance, void (T::* action)(S param))
	{
		_instance = instance;
		_action = action;
	}

	void operator()(S param) override
	{
		(_instance->*_action)(param);
	}

	inline bool operator==(IDelegate<S>* other) override
	{
		Delegate* otherDelegate = dynamic_cast<Delegate<T,S>*>(other);
		if (otherDelegate == nullptr)
			return false;
		return (this->_action == otherDelegate->_action) &&
			(this->_instance == otherDelegate->_instance);
	}

private:
	void (T::* _action)(S param);
	T* _instance;
};


template<typename S>
class Event
{
public:
	Event() = default;

	void operator+=(IDelegate<S>* delegate)
	{
		_invocationList.insert(delegate);
	}

	void operator-=(IDelegate<S>* delegate)
	{
		_invocationList.erase(delegate);
	}

	void Invoke(S param) const 
	{
		for (const auto& invoker : _invocationList)
		{
			(*invoker)(param);
		}
	}

	IDelegate<S>* operator=(IDelegate<S>* delegate) = delete;

private:
	typedef std::unordered_set<IDelegate<S>*> Delegates;
	Delegates _invocationList;
};


