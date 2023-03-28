#pragma once

#include <chrono>
#include <thread>
#include <functional>
#include <memory>
#include <vector>

#include "iostream"
#include "Core/Core.h"
#include "Defines.h"
#include "Core/KeyCode.h"
#include "Core/MouseButton.h"
#include "ECS/ECS.h"
#include "ECS/ISystem.h"
#include "Window.h"

#include "glad/glad.h"
#include "../3rd/glfw/include/GLFW/glfw3.h"
#include "Event.h"

//#include "imgui.h"
//
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <imgui_internal.h>
//#include <imgui.h>
//#include "imgui_impl_opengl3_loader.h"

namespace CW
{
	class CW_API App : enable_shared_from_this<App>
	{
	public:
		static App* Create(const std::string& name = "")
		{
			ECS::Instance().Init();
			return new App(name);
		}
		~App();
		 
		template<typename T = ISystem, typename... C>
		App* AddSystem()
		{
			shared_ptr<ISystem> system = ECS::Instance().RegisterSystem<T, C...>();
			_systems.push_back(system);
			return this;
		}

		void Run(const unsigned int height, const unsigned int width);


	private:
		explicit App(const std::string& name);

		Window _window;

		bool _isRunning;
		float _backColor[4];
		std::string _name;

		std::vector<shared_ptr<ISystem>> _systems{};
	};
}
