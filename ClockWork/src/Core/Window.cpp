#include "Window.h"

namespace CW::Core
{
	Window::Window() :
		m_window(NULL)
	{
		m_IsRunning = false;
	}

	Window::~Window()
	{
	}

	bool Window::InitWindow(const std::string name, const unsigned int width, const unsigned int height)
	{
		if (!glfwInit())
		{
			std::cout << " failure on initializing glfw " << std::endl;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (!m_window)
		{
			std::cout << "failure on window creation" << std::endl;
			return false;
		}

		glfwMakeContextCurrent(m_window);			
		glfwSwapInterval(1);				
		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int c, int d){glViewport(0,0, c, d); });

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_IsRunning = true;
		return m_IsRunning;
	}

	void Window::Update()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			ProcessInput();
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
		Terminate();
	}

	void Window::Terminate()
	{
		m_IsRunning = false;
		glfwTerminate();
	}
	 
	void Window::ProcessInput()
	{
		// this function needs to fire the events through a queue based on eventlist with key, mouse buttons etc.

		if (glfwGetKey(m_window, CW::Key::KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(m_window, true);
		}
	}

	void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
