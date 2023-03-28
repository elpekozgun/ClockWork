#pragma once
#include "Core/Defines.h"
#include "Core/Core.h"
#include "Core/KeyCode.h"
#include "ECS.h"

#include <set>
#include <typeinfo>


namespace CW
{
	class CW_API ISystem
	{
	public:
		virtual void Update(float deltaTime) = 0;

		ComponentMask _systemMask;

	protected:
		std::set<EntityId> _entities;

		float _updateRate = 50;
		float _currentDelta = 0;


		friend class SystemManager;
	};

	template<typename T>
	concept ISystemConcept = std::derived_from<T, ISystem>;
}