#pragma once

#include "Event.h"

class ISystem
{
public:
	ISystem(float updateRate) : _updateRate(updateRate) {}
	virtual ~ISystem() {};
	
	virtual void Update(float deltaTime) = 0;
	//{
	//	_deltaTime += deltaTime;
	//	if (_deltaTime >= _updateRate)
	//	{
	//		OnUpdated.Invoke(_deltaTime);
	//		_deltaTime = 0;
	//	}
	//};

	virtual void Shutdown() const = 0;
	virtual float GetUpdateRate() const = 0;

	Event<float> OnUpdated;

protected:
	float _updateRate;
	float _deltaTime = 0;
};