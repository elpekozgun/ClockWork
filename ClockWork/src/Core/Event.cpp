#include "Event.h"

void Event::Invoke() const
{
	for (const auto& invoker : _invocationList)
	{
		(*invoker)();
	}
}
