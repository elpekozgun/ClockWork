#pragma once
#include <iostream>
#include <bitset>
#include <chrono>

namespace CW
{
	// generic types
	typedef std::uint32_t uint;
	typedef std::uint8_t ubyte;
	typedef std::uint64_t ulonglong;

	//ecs types
	constexpr uint MAX_ENTITY = 30000;
	constexpr unsigned short MAX_COMPONENT = 1250;

	typedef uint EntityId;
	typedef ubyte ComponentType;
	typedef std::bitset<MAX_COMPONENT> ComponentMask;

	constexpr uint SCREEN_WIDTH = 1280;
	constexpr uint SCREEN_HEIGHT = 720;

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::duration<float, std::milli> MilliFloat;
	typedef std::chrono::duration<float, std::chrono::seconds> secondFloat;
}
