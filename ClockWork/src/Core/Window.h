#pragma once 

#include <string>
#include <iostream>

#include "Core.h"
#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "Core/KeyCode.h"
#include "InputState.h"
#include "Defines.h"

namespace CW
{
	class CW_API Window
	{
	public:
		Window(const std::string& name) :  _name(name), _window(nullptr){ }
		~Window();

		void Setup(int width, int height);
		void Update();
		void Shutdown();
		void SetTitle(const char* title)
		{
			glfwSetWindowTitle(_window, title);
		}

	private:
		void HandleCursor();

		GLFWwindow* _window;
		std::string _name;

		bool _firstClick = true;
		double _lastX = 0;
		double _lastY = 0;
	};




}


