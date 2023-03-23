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

		friend class SystemManager;
	};

}