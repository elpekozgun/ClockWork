#include "Application.h"


namespace CW::Core
{
	void TimedStartOnThread(std::function<void()> func, const uint32_t interval)
	{
		std::thread([func, interval]
		{
			while (true)
			{
				func();
				auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
				std::this_thread::sleep_until(x);
			}
		}).detach();
	}

	void Application::UpdatePhysics()
	{
		std::cout << "thread id " << std::this_thread::get_id() << std::endl;
		while (_isRunning)
		{
			OnPhysics.Invoke("physics updated");
			//std::cout << "hello from physics thread\n";
			auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
			std::this_thread::sleep_until(x);
		}
	}

	void Application::RunPhysicsThread()
	{
		std::thread(&Application::UpdatePhysics, this).detach();
	}

	Application::Application(const std::string& name) :
		_name(name), _window(nullptr), _backColor{0.15f, 0.15f, 0.15f, 1.0f}
	{
		std::cout << "main thread id " << std::this_thread::get_id() << std::endl;

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
		
		_isRunning = true;

		RunPhysicsThread();


		while (_isRunning)
		{
			auto tStart = std::chrono::system_clock::now();

			glfwPollEvents();
			
			glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			
			HandleInput();

			UpdateUI();

			glfwSwapBuffers(_window);
			
			auto tEnd = std::chrono::system_clock::now();

			auto ms = (std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart)).count();

			OnRender.Invoke((int)(1000.0f / ms));
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

		}
	}

	void Application::FrameBufferSizeCallback(GLFWwindow* Application, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
