#pragma once

#include "iostream"
#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/KeyCode.h"
#include "Core/MouseButton.h"

#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "imgui.h"

namespace CW::Core
{
	class CW_API Application
	{
	private:
		GLFWwindow* _window;
		ImGuiIO* io = nullptr;

		bool _isRunning;
		float _backColor[4];
		std::string _name;

		void UpdateUI();
		void Render();
		void Simulate();

	public:
		explicit Application(const std::string& name);
		~Application();

		void Run(const unsigned int height, const unsigned int width);
		void HandleInput();
		void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	};

}