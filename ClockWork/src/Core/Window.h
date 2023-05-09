#pragma once 

#include <string>
#include <iostream>
#include <functional>

#include "Core.h"
#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "Core/KeyCode.h"
#include "Input.h"
#include "Defines.h"

#include "Gui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3_loader.h>

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
		void AddGuiCallback(const std::function<void()>& guiFunction);

		bool ShouldClose = false;

	private:
		void HandleCursor();

		GLFWwindow* _window;
		std::string _name;

		std::vector<std::function<void()>> GuiCallbacks;

		bool _firstClick = true;
		double _lastX = 0;
		double _lastY = 0;
	};




}


