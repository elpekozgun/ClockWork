#pragma once
#include <iostream>
#include <bitset>
#include <set>
#include "Core/Core.h"

typedef std::uint32_t Entity;
const Entity MAX_ENTITIES = 5000;

typedef std::uint8_t ComponentType;
const ComponentType MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Mask;

class CW_API System
{
public:
	virtual void Update(float deltaTime) = 0;

	std::set<Entity> _entities;
};

