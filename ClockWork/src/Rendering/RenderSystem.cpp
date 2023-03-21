#include "RenderSystem.h"

#include <iostream>

void RenderSystem::Update(float deltaTime)
{
	_deltaTime += deltaTime;
	if (_deltaTime >= _updateRate)
	{
		OnUpdated.Invoke(_deltaTime);
		_deltaTime = 0;
	}
}

float RenderSystem::GetUpdateRate() const
{
	return _deltaTime;
}

void RenderSystem::Shutdown() const
{

}
