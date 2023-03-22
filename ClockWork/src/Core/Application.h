#pragma once

#include "iostream"
#include "Core/Core.h"
#include "Defines.h"
#include "Core/KeyCode.h"
#include "Core/MouseButton.h"
#include "Physics/PhysicsSystem.h"
#include "Rendering/RenderSystem.h"
#include "ECS/ECS.h"

#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
//#include "imgui.h"
//
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <imgui_internal.h>
//#include <imgui.h>
//#include "imgui_impl_opengl3_loader.h"

#include <chrono>
#include <thread>
#include <functional>
#include <memory>
#include "Event.h"
#include "ECS/System/PseudoPhysicsSystem.h"	


namespace CW::Core
{
	class CW_API Application
	{
	public:
		explicit Application(const std::string& name);
		~Application();

		void Run(const unsigned int height, const unsigned int width);

	private:
		GLFWwindow* _window;

		bool _isRunning;
		float _backColor[4];
		std::string _name;

		void HandleInput();
		void Render();
		//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

	};

}

//namespace CW::Core
//{
//	class CW_API Application
//	{
//	public:
//		explicit Application(const std::string& name);
//		~Application();
//
//		void Run(const unsigned int height, const unsigned int width);
//
//		Event<int> OnRender;
//		Event<std::string> OnPhysics;
//
//	private:
//		GLFWwindow* _window;
//		ImGuiIO* io = nullptr;
//
//		bool _isRunning;
//		float _backColor[4];
//		std::string _name;
//
//		void HandleInput();
//		void UpdateUI();
//		void Render();
//		void Simulate();
//		void UpdatePhysics();
//		void RunPhysicsThread();
//		void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
//
//	
//	};
//
//}