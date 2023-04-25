#include "App.h"
#include "ThreadPool.h"
#include "Macro.h"

namespace CW
{
	App::App(const std::string& name) :
		_name(name), _backColor{ 0.15f, 0.15f, 0.15f, 1.0f }, _window(name)
	{
		_ecs.Init();
		_window.Setup(1920, 1080);
	}

	App::~App()
	{
		glfwTerminate();
	}

	void App::Run(const unsigned int width, const unsigned int height)
	{
		double windowReset = 0.0;

		double prevTime = 0.0;
		double currentTime = 0.0;
		int frameCount = 0;
		double dt;

		//ThreadPool tp(_systems.size());

		//thread renderThread;

		while (!_window.ShouldClose)
		{
			currentTime = glfwGetTime();
			dt = currentTime - prevTime;
			
			windowReset += dt;
			if (windowReset >= 0.25)
			{
				std::string FPS = std::to_string((1.0 / dt));
				std::string ms = std::to_string(dt * 1000);
				std::string title = _name + " - FPS: " + FPS + " / ms: " + ms + " / dt: " + std::to_string(dt);
				_window.SetTitle(title.c_str());
				windowReset = 0;
			}

			for (auto& system : _systems)
			{
				system->Update(dt);
				//TIMEIT(system->Update(dt), system->Name);
			}

			//for (size_t i = 1; i < _systems.size() - 1; i++)
			//{
			//	auto& system = _systems[i];
			//	tp.Push([&system, dt, i](size_t) 
			//	{ 
			//		system->Update(dt); }
			//	);
			//}
			//tp.Wait();
			//_systems[0]->Update(dt);
			//_systems[5]->Update(dt);

			prevTime = currentTime;





			_window.Update();
		}
	}
}
