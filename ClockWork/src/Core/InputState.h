#pragma once
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include <bitset>
#include "KeyCode.h"
#include "MouseButton.h"

struct CW_API InputState
{
public:
	static InputState& Instance() 
	{
		static InputState instance;
		return instance;
	}

	inline void SetKeyDown(const int& key) { _keyboard.set(key, true); }
	inline void SetKeyUp(int key) { _keyboard.set(key, false); }
	inline bool IsKeyDown(int key) { return _keyboard.test(key); }

	inline void SetMouseDown(int key) { _mouse.set(key,true); }
	inline void SetMouseUp(int  key) { _mouse.set(key, false); }
	inline bool IsMouseDown(int key) { return _mouse.test(key); }

	inline void Clear() { _keyboard.reset(); _mouse.reset(); }

	double MouseDX = 0;
	double MouseDY = 0;
	bool FirstClick = true;

private:
		std::bitset<GLFW_KEY_LAST> _keyboard;
		std::bitset<GLFW_MOUSE_BUTTON_LAST> _mouse;


		InputState() {};
};