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


namespace CW
{
	class CW_API App : enable_shared_from_this<App>
	{
	public:
		static App* Create(const std::string& name = "")
		{
			return new App(name);
		}
		~App();
		 
		template<typename T = ISystem, typename... C>
		App* AddSystem()
		{
			shared_ptr<ISystem> system = _ecs.RegisterSystem<T, C...>();
			_systems.push_back(system);
			_window.AddGuiCallback([system]() {system->OnGui(); });
			return this;
		}

		void Run(const unsigned int height, const unsigned int width);

		ECS& GetECS()
		{
			return _ecs;
		}

	private:
		explicit App(const std::string& name);

		Window _window;

		float _backColor[4];
		std::string _name;

		ECS _ecs;

		std::vector<shared_ptr<ISystem>> _systems{};
	};
}
