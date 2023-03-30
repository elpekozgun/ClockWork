#include "App.h"
#include "Graphics/TemporaryRenderStuff.h"

namespace CW
{
	App::App(const std::string& name) :
		_name(name), _backColor{ 0.15f, 0.15f, 0.15f, 1.0f }, _window(name)
	{
		ECS::Instance().Init();
		_isRunning = false;
	}

	App::~App()
	{
		_isRunning = false;
		glfwTerminate();
	}

	void App::Run(const unsigned int width, const unsigned int height)
	{
		_window.Setup(width, height);

		TempRender tempRender;

		_isRunning = true;

		float dt = 0;
		while (_isRunning)
		{
			
			auto tStart = Clock::now();

			for (const auto& system : _systems)
			{
				system->Update(dt);
			}
			tempRender.Render(dt);

			auto tEnd = Clock::now();
			dt = MilliFloat(tEnd - tStart).count();

			_window.Update();
		}
	}
}
