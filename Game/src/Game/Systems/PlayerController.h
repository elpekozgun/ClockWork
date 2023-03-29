#pragma once

#include "ClockWork.h"

class PlayerController : public CW::ISystem
{
	virtual void Update(float dt) override;
};
