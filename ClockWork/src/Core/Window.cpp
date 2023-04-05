#include "Window.h"

namespace CW
{
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* app, int c, int d) 
		{
			glViewport(0, 0, c, d);
		});
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return;


		glfwSetKeyCallback(_window, KeyCallback);
		glfwSetMouseButtonCallback(_window, MouseButtonCallback);

		_lastX = width / 2;
		_lastY = height / 2;
	}

	Window::~Window()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::Update()
	{
		glfwPollEvents();
		HandleCursor();
		glfwSwapBuffers(_window);
	}

	void Window::HandleCursor()
	{
		auto& input = Input::Instance();

		if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(_window, &xpos, &ypos);
			if (_firstClick)
			{
				_lastX = xpos;
				_lastY = ypos;
				_firstClick = false;
			}

			float xoffset = xpos - _lastX;
			float yoffset = _lastY - ypos; 

			_lastX = xpos;
			_lastY = ypos;

			input.MouseDX = xoffset;
			input.MouseDY = yoffset;
		}
		else if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			_firstClick = true;
		}
	}

	void Window::Shutdown()
	{
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto& input = Input::Instance();

		if (key == GLFW_KEY_UNKNOWN)
			return;

		if (action == GLFW_PRESS)
		{
			input.SetKeyDown(key);
			input.SetKeyPressed(key);
		}
		else if (action == GLFW_RELEASE)
		{
			input.SetKeyUp(key);
		}
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto& input = Input::Instance();

		if (action == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			input.SetMouseDown(button);
			//input.SetMouseClicked(button);
		}
		else if (action == GLFW_RELEASE)
		{
			input.SetMouseUp(button);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

}


