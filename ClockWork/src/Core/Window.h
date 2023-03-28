#pragma once 

#include <string>
#include <iostream>

#include "Core.h"
#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "Core/KeyCode.h"
#include "InputState.h"

namespace CW
{
	class CW_API Window
	{
	public:
		Window(const std::string& name) : 
			_name(name), _window(nullptr), _backColor{ 0.15f, 0.15f, 0.15f, 1.0f } { }
		~Window();

		void Setup(int width, int height);
		void Update();
		void Shutdown();

	private:
		GLFWwindow* _window;
		float _backColor[4];
		std::string _name;
	};




}


