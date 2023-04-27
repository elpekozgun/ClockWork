#pragma once
#include "Window.h"

namespace CW
{
	class Renderer
	{
	public:
		Renderer(GLFWwindow* window) : _window(window)
		{
			glfwMakeContextCurrent(window);
		}

		void Run()
		{
			while (!glfwWindowShouldClose(_window))
			{
				std::thread rendererThread(&Renderer::Render, this);
				rendererThread.join();

				glfwPollEvents();
				glfwSwapBuffers(_window);
			}
		}

		void Render()
		{

		}


	private:
		GLFWwindow* _window = nullptr;
		std::mutex _lock;
	};
}