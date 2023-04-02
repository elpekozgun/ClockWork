#pragma once
#include "Core/Core.h"
#include "Core/Defines.h"

#include <set>
#include <typeinfo>


namespace CW
{
	class CW_API ISystem
	{
	public:
		virtual void Update(float dt) = 0;

		ComponentMask _systemMask;

	protected:
		std::set<EntityId> _entities;

		float _updateRate = 0.01666666f;
		float _currentDelta = 0;

		friend class SystemManager;
	};

	template<typename T>
	concept ISystemConcept = std::derived_from<T, ISystem>;
}