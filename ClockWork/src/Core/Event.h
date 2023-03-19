#pragma once
#include <unordered_set>
#include <functional>

using namespace std;


template<typename T>
class Delegate
{
public:
	typedef function<void(T)> Act;
	Act Action;

	//typedef void (*Action)(T);

	//inline void Set(const Action action)
	inline void Set(function<void(T)> action)
	{
		Action = action;
	}

	//inline void Set(void*() action)
	//{
	//	Action = action;
	//}

	template<typename T>
	//inline void operator=(void (*action)(T))
	inline void operator=(function<void(T)> action)
	{
		Action = action;
	}
};


template<typename T>
class Event
{
	typedef void (*Action)(T);

public:
	inline void operator+=(Delegate<T> del) 
	{
		//_invocationList.push_back(&del);
		_invocationList.insert(&del);
	}

	inline void operator+=(Action act)
	{
		Delegate<T> del;
		del.Set(act);

		_invocationList.insert(del);
	}

	inline void operator-=(Delegate<T> del) 
	{
		_invocationList.erase(&del); 
	}
	inline void operator()(T param) = delete;
	
	inline void Invoke(T param) 
	{ 
		for (const Delegate<T>* const itr : _invocationList)
		{
			itr->Action(param);
		}
	}
	
	//Event& operator=(Delegate<T> delegate) delete;

private:
	unordered_set<Delegate<T>*> _invocationList;
};

