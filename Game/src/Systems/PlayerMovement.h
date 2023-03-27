#pragma once

#include "ClockWork.h"

struct TransformComponent
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

struct InputComponent
{
	bool Up;
	bool Down;
	bool Right;
	bool Left;
	bool Attack;
	bool isReleased;
	bool isHolding;
};

class PlayerController : public CW::ISystem
{
	virtual void Update(float deltaTime) override;
	virtual void GetInput(int input) override;
};

// we need to way to notify the input system that a key is pressed.
class InputSystem : public CW::ISystem
{
	// Inherited via ISystem
	virtual void Update(float deltaTime) override;
	virtual void GetInput(int input) override;
};

