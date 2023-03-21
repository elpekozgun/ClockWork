#pragma once

#include "Core/ISystem.h"

class RenderSystem : public ISystem
{
public:
	using ISystem::ISystem;

	void Update(float deltaTime) override;
	void Shutdown() const override;
	float GetUpdateRate() const override;
};