#include "Application.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui.h>
#include "imgui_impl_opengl3_loader.h"

namespace CW::Core
{

	Application::Application(const std::string& name) :
		_name(name), _window(nullptr), _backColor{0.15f, 0.15f, 0.15f, 1.0f}
	{
		_isRunning = false;
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		_isRunning = false;

		glfwTerminate();
	}

	void Application::Run(const unsigned int width, const unsigned int height)
	{
		const char* glslVersion = "#version 460";

		if (!glfwInit())
		{
			std::cout << " failure on initializing glfw " << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(width, height, _name.c_str(), NULL, NULL);
		if (!_window)
		{
			std::cout << "failure on Application creation" << std::endl;
			return;
		}

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(1);
		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* Application, int c, int d) {glViewport(0, 0, c, d); });

		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();
		ImGui::StyleColorsDark();
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init(glslVersion);

		//glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
		
		_isRunning = true;
		while (_isRunning)
		{
			glfwPollEvents();

			glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			HandleInput();

			UpdateUI();

			glfwSwapBuffers(_window);
		}
	}

	void Application::UpdateUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Color Picker");
		ImGui::ColorPicker3("back color", _backColor);
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::Render()
	{
	}

	void Application::Simulate()
	{
	}

	void Application::HandleInput()
	{
		if (glfwWindowShouldClose(_window))
			_isRunning = false;

		if (glfwGetKey(_window, CW::Key::KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(_window, true);
			_isRunning = false;
		}

		if (glfwGetKey(_window, CW::Key::KEY_SPACE))
		{ 
			//_showText= !_showText;
		}
	}

	void Application::FrameBufferSizeCallback(GLFWwindow* Application, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
