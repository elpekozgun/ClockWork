#pragma once
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include <bitset>
#include "KeyCode.h"
#include "MouseButton.h"
#include "Core/Core.h"

struct CW_API Input
{
public:
	static Input& Instance() 
	{
		static Input instance;
		return instance;
	}

	inline bool GetKeyPressed(const int& key)
	{
		bool result = false;
		if (_keyPressed.test(key))
		{
			_keyPressed.set(key, false);
			result = true;
		}
		return result;
	}

	inline bool GetKeyDown(const int& key) const
	{
		return _key.test(key);
	}

	inline void SetKeyDown(const int& key) 
	{ 
		_key.set(key, true); 
	}
	
	inline void SetKeyPressed(const int& key) 
	{ 
		if (!_keyPressed.test(key))
		{
			_key.set(key, true);
			_keyPressed.set(key, true);

		}
	}

	inline void SetKeyUp(int key) 
	{ 
		_key.set(key, false); 
		_keyPressed.set(key, false);
	}


	inline bool GetMouseClicked(int key)
	{
		return _mouseClicked.test(key);
	}
	
	inline bool GetMouseDown(int key)
	{
		return _mouse.test(key);
	}

	inline void SetMouseClicked(int key) 
	{ 
		if (!_mouseClicked.test(key))
		{
			_mouse.set(key, true);
			_mouseClicked.set(key,true);
		}
	}

	inline void SetMouseDown(int key)
	{
		_mouse.set(key, true);
	}

	inline void SetMouseUp(int  key) 
	{ 
		_mouseClicked.set(key, false);
		_mouse.set(key, false); 
	}

	inline void Clear() { _key.reset(); _mouse.reset(); }

	double MouseDX = 0;
	double MouseDY = 0;
	bool FirstClick = true;

private:
		std::bitset<GLFW_KEY_LAST> _key;
		std::bitset<GLFW_KEY_LAST> _keyPressed;
		std::bitset<GLFW_MOUSE_BUTTON_LAST> _mouse;
		std::bitset<GLFW_MOUSE_BUTTON_LAST> _mouseClicked;

		Input() {};
};