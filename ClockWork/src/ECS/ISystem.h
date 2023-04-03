#pragma once
#include "Core/Core.h"
#include "Core/Defines.h"
#include <set>

namespace CW
{
	class ECS;

	class CW_API ISystem
	{
	public:
		virtual void Update(float dt) = 0;

		ComponentMask _systemMask;
		ECS* _ecs = nullptr;

		std::set<EntityId> _entities;
	protected:

		float _updateRate = 0.01666666f;
		float _currentDelta = 0;

		friend class SystemManager;
	};

	template<typename T>
	concept ISystemConcept = std::derived_from<T, ISystem>;
}