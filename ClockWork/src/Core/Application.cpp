#include "Application.h"




namespace CW::Core
{
	Application::Application(const std::string& name) :
		_name(name), _window(nullptr), _backColor{ 0.15f, 0.15f, 0.15f, 1.0f }
	{
		_isRunning = false;
	}

	Application::~Application()
	{
		_isRunning = false;
		glfwTerminate();
	}

	int lastKey;

	void KeyWrapper(GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		lastKey = key;
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
		glfwSetKeyCallback(_window, [](GLFWwindow* app, int key, int scancode, int action, int mods) {lastKey = key; });

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return;
		}

		ECS& ecs = ECS::Instance();
		ecs.Init();

		auto physicsSystem = ecs.RegisterSystem<PseudoPhysicsSystem>();
		{
			ComponentMask mask;
			mask.set(ecs.GetComponentType<GravityComponent>());
			mask.set(ecs.GetComponentType<TransformComponent>());
			mask.set(ecs.GetComponentType<RigidBodyComponent>());
			ecs.SetSystemMask<PseudoPhysicsSystem>(mask);
		}

		std::vector<EntityId> entities(5);


		_isRunning = true;

		while (_isRunning)
		{
			auto tStart = std::chrono::high_resolution_clock::now();

			HandleInput();

			glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glfwPollEvents();


			auto tEnd = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float, milli> elapsed = tEnd - tStart;
			float deltaTime = elapsed.count();

			ECS::Instance().HandleInput(lastKey);

			physicsSystem->Update(deltaTime);

			// Add System manager to iterate over all.
			/*_physicsSystem->Update(deltaTime);
			_renderSystem->Update(deltaTime);*/

			glfwSwapBuffers(_window);


			lastKey = 0;

			//OnRender.Invoke((int)(1000.0f / ms));
		}
	}



	void Application::Render()
	{

	}

	void Application::HandleInput()
	{
		if (glfwWindowShouldClose(_window))
			_isRunning = false;

		if (glfwGetKey(_window, CW::KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(_window, true);
			_isRunning = false;
		}

		if (glfwGetKey(_window, CW::KEY_SPACE))
		{

		}
	}

	//void Application::FrameBufferSizeCallback(GLFWwindow* Application, int width, int height)
	//{
	//	glViewport(0, 0, width, height);
	//}


}

//
//namespace CW::Core
//{
//	void Application::UpdatePhysics()
//	{
//		std::cout << "thread id " << std::this_thread::get_id() << std::endl;
//		while (_isRunning)
//		{
//			//OnPhysics.Invoke("physics updated");
//			//std::cout << "hello from physics thread\n";
//			auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
//			std::this_thread::sleep_until(x);
//		}
//	}
//
//	void Application::RunPhysicsThread()
//	{
//		std::thread(&Application::UpdatePhysics, this).detach();
//	}
//
//	void Application::Log(float s)
//	{
//		std::cout << s << "\n";
//	}
//
//	Application::Application(const std::string& name) :
//		_name(name), _window(nullptr), _backColor{0.15f, 0.15f, 0.15f, 1.0f}
//	{
//		std::cout << "main thread id " << std::this_thread::get_id() << std::endl;
//
//		_renderSystem = std::make_unique<RenderSystem>(0);
//		_physicsSystem = std::make_unique<PhysicsSystem>(50);
//
//		OnRenderCompleted.Set(this, &Application::Log);
//		OnPhysicsCompleted.Set(this,&Application::Log);
//
//		_renderSystem.get()->OnUpdated += &OnRenderCompleted;
//		_physicsSystem.get()->OnUpdated += &OnPhysicsCompleted;
//
//		_isRunning = false;
//	}
//
//	Application::~Application()
//	{
//		ImGui_ImplOpenGL3_Shutdown();
//		ImGui_ImplGlfw_Shutdown();
//		ImGui::DestroyContext();
//		_isRunning = false;
//
//		glfwTerminate();
//	}
//
//	void Application::Run(const unsigned int width, const unsigned int height)
//	{
//		const char* glslVersion = "#version 460";
//
//		if (!glfwInit())
//		{
//			std::cout << " failure on initializing glfw " << std::endl;
//		}
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//		_window = glfwCreateWindow(width, height, _name.c_str(), NULL, NULL);
//		if (!_window)
//		{
//			std::cout << "failure on Application creation" << std::endl;
//			return;
//		}
//
//		glfwMakeContextCurrent(_window);
//		glfwSwapInterval(1);
//		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* Application, int c, int d) {glViewport(0, 0, c, d); });
//
//		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//
//		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//		{
//			return;
//		}
//
//		IMGUI_CHECKVERSION();
//		ImGui::CreateContext();
//		io = &ImGui::GetIO();
//		ImGui::StyleColorsDark();
//		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//		io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
//
//		ImGui_ImplGlfw_InitForOpenGL(_window, true);
//		ImGui_ImplOpenGL3_Init(glslVersion);
//		
//		_isRunning = true;
//
//		//RunPhysicsThread();
//
//		while (_isRunning)
//		{
//			auto tStart = std::chrono::high_resolution_clock::now();
//
//			glfwPollEvents();
//			
//			glClearColor(_backColor[0], _backColor[1], _backColor[2], _backColor[3]);
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//			
//			HandleInput();
//
//			UpdateUI();
//
//			glfwSwapBuffers(_window);
//			
//			auto tEnd = std::chrono::high_resolution_clock::now();
//
//			std::chrono::duration<float,milli> elapsed = tEnd - tStart;
//			float deltaTime = elapsed.count();
//
//			// Add System manager to iterate over all.
//			_physicsSystem->Update(deltaTime);
//			_renderSystem->Update(deltaTime);
//
//
//			//OnRender.Invoke((int)(1000.0f / ms));
//		}
//	}
//
//	void Application::UpdateUI()
//	{
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		ImGui::Begin("Color Picker");
//		ImGui::ColorPicker3("back color", _backColor);
//		ImGui::End();
//
//		ImGui::EndFrame();
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//		{
//			GLFWwindow* backup_current_context = glfwGetCurrentContext();
//			ImGui::UpdatePlatformWindows();
//			ImGui::RenderPlatformWindowsDefault();
//			glfwMakeContextCurrent(backup_current_context);
//		}
//	}
//
//	void Application::Render()
//	{
//	}
//
//	void Application::Simulate()
//	{
//	}
//
//	void Application::HandleInput()
//	{
//		if (glfwWindowShouldClose(_window))
//			_isRunning = false;
//
//		if (glfwGetKey(_window, CW::KEY_ESCAPE))
//		{
//			glfwSetWindowShouldClose(_window, true);
//			_isRunning = false;
//		}
//
//		if (glfwGetKey(_window, CW::KEY_SPACE))
//		{ 
//
//		}
//	}
//
//	void Application::FrameBufferSizeCallback(GLFWwindow* Application, int width, int height)
//	{
//		glViewport(0, 0, width, height);
//	}
//
//}
