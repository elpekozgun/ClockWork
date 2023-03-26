#pragma once
#include "Core/Defines.h"
#include "Core/Core.h"
#include "Core/KeyCode.h"

#include <set>



namespace CW
{
	class CW_API ISystem
	{
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void GetInput(int input) = 0;

	protected:
		void RemoveEntity(EntityId entity)
		{
			//auto lastEntity = _entities[_entities.size() - 1];

			//_entities[entity] = 
		}

		std::set<EntityId> _entities;
		ComponentMask _systemMask;

		float _updateRate = 50;
		float _currentDelta = 0;

		friend class SystemManager;
	};

	template<typename T>
	concept ISystemConcept = std::derived_from<T, ISystem>;
}