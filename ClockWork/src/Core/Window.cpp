#include "Window.h"

namespace CW
{
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseCallBack(GLFWwindow* window, int button, int action, int mods);

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
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* app, int c, int d) {glViewport(0, 0, c, d); });
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return;

		glfwSetKeyCallback(_window, KeyCallback);
		glfwSetMouseButtonCallback(_window, MouseCallBack);


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

		glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(_window);
	}

	void Window::Shutdown()
	{
		glfwPollEvents();
	}


	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto& input = InputState::Instance();

		if (key == GLFW_KEY_UNKNOWN)
			return;

		if (action == GLFW_PRESS)
		{
			input.SetKeyDown(key);
		}
		else if (action == GLFW_RELEASE)
		{
			input.SetKeyUp(key);
		}
	}

	void MouseCallBack(GLFWwindow* window, int button, int action, int mods)
	{
		auto& input = InputState::Instance();

		if (action == GLFW_PRESS)
		{
			input.SetMouseDown(button);
		}
		else if (action == GLFW_RELEASE)
		{
			input.SetMouseUp(button);
		}
	}


}


