#include "App.h"


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

	int lastKey;

	void KeyWrapper(GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		lastKey = key;
	}

	void App::Run(const unsigned int width, const unsigned int height)
	{
		_window.Setup(width, height);

		_isRunning = true;

		float dt = 0;
		while (_isRunning)
		{
			auto tStart = std::chrono::high_resolution_clock::now();

			for (const auto& system : _systems)
			{
				system->Update(dt);
			}

			std::cout << 1000 / dt << "\n";

			auto tEnd = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::milli>(tEnd - tStart).count();

			_window.Update();
		}
	}

	void App::Render()
	{

	}

	//void App::HandleInput()
	//{
	//	glfwPollEvents();

	//	if (glfwWindowShouldClose(_window))
	//		_isRunning = false;

	//	if (glfwGetKey(_window, CW::KEY_ESCAPE))
	//	{
	//		glfwSetWindowShouldClose(_window, true);
	//		_isRunning = false;
	//	}

	//	if (glfwGetKey(_window, CW::KEY_SPACE))
	//	{

	//	}
	//}
}
