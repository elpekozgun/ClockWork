#include "Window.h"

namespace CW
{
	void Window::Setup(int width, int height)
	{
		const char* glslVersion = "#version 460";

		if (!glfwInit())
		{
			std::cout << " failure on initializing glfw " << std::endl;
		}

		_window = glfwCreateWindow(width, height, _name.c_str(), NULL, NULL);
		if (!_window)
		{
			std::cout << "failure on window creation" << std::endl;
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(0);
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* App, int c, int d) {glViewport(0, 0, c, d); });
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return;

		glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
		glEnable(GL_DEPTH_TEST);
	}

	Window::~Window()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}