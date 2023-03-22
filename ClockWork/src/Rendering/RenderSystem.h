#pragma once

#include "Core/OldSystem.h"

namespace CW
{
	class RenderSystem : public OldSystem
	{
	public:
		using OldSystem::OldSystem;

		void Update(float deltaTime) override;
		void Shutdown() const override;
		float GetUpdateRate() const override;
	};
}