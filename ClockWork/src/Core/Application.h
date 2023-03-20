#pragma once

#include "iostream"
#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/KeyCode.h"
#include "Core/MouseButton.h"

#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "imgui.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui.h>
#include "imgui_impl_opengl3_loader.h"

#include <chrono>
#include <thread>
#include <functional>
#include "Event.h"

namespace CW::Core
{
	class CW_API Application
	{
	public:
		explicit Application(const std::string& name);
		~Application();

		void Run(const unsigned int height, const unsigned int width);
		void HandleInput();
		void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

		Event OnRender;
		Event OnPhysics;

	private:
		GLFWwindow* _window;
		ImGuiIO* io = nullptr;

		bool _isRunning;
		float _backColor[4];
		std::string _name;

		void UpdateUI();
		void Render();
		void Simulate();
		void UpdatePhysics();
		void RunPhysicsThread();

	
	};

}