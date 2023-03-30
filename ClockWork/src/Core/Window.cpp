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
			std::cout << " failure on initializing glfw " << std::endl; return;
		}

		_window = glfwCreateWindow(width, height, _name.c_str(), NULL, NULL);
		if (!_window)
		{
			std::cout << "failure on window creation" << std::endl; return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

		//glfwSetWindowSize(_window,CW::SCREEN_HEIGHT, CW::SCREEN_HEIGHT);
		glfwMakeContextCurrent(_window);
		glfwSwapInterval(0);
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* app, int c, int d) {glViewport(0, 0, c, d); });
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return;

		glEnable(GL_DEPTH_TEST);

		glfwSetKeyCallback(_window, KeyCallback);
		glfwSetMouseButtonCallback(_window, MouseCallBack);
	}

	Window::~Window()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	float elapsed;
	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}

	void Window::Shutdown()
	{
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


