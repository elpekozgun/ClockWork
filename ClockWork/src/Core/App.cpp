#include "App.h"
#include "Graphics/TemporaryRenderStuff.h"

namespace CW
{
	App::App(const std::string& name) :
		_name(name), _backColor{ 0.15f, 0.15f, 0.15f, 1.0f }, _window(name)
	{
		_window.Setup(1920, 1080);

		_ecs.Init();
		_isRunning = false;
	}

	App::~App()
	{
		_isRunning = false;
		glfwTerminate();
	}

	void App::Run(const unsigned int width, const unsigned int height)
	{
		//_window.Setup(width, height);

		//TempRender tempRender;

		_isRunning = true;


		double prevTime = 0.0;
		double currentTime = 0.0;
		int frameCount = 0;
		double dt;

		while (_isRunning)
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

			for (const auto& system : _systems)
			{
				system->Update(dt);
			}

			_window.Update();
		}
	}
}
