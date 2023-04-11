#include "App.h"
#include "ThreadPool.h"

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
		double prevTime = 0.0;
		double currentTime = 0.0;
		int frameCount = 0;
		double dt;

		ThreadPool tp(_systems.size());

		thread renderThread;

		while (!_window.ShouldClose)
		{
			currentTime = glfwGetTime();
			dt = currentTime - prevTime;
			frameCount++;
			if (dt >= 1.0 / 60.0)
			{
				std::string FPS = std::to_string((1.0 / dt) * frameCount);
				std::string ms = std::to_string(dt / frameCount * 1000);
				std::string title = _name + " - FPS: " + FPS + " / ms: " + ms + " / dt: " + std::to_string(dt);
				_window.SetTitle(title.c_str());
				prevTime = currentTime;
				frameCount = 0;
			}
			//for (auto& system : _systems)
			//{
			//	system->Update(dt);
			//}

			for (size_t i = 1; i < _systems.size(); i++)
			{
				auto& system = _systems[i];
				tp.Push([&system, dt, i](size_t) 
				{ 
					system->Update(dt); });
			}
			tp.Wait();
			_systems[0]->Update(dt);



			_window.Update();
		}
	}
}
