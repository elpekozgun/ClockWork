#pragma once

#include "iostream"
#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/KeyCode.h"
#include "Core/MouseButton.h//"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
//#include "ShaderFactory.h"
//#include "Entity/Component/Mesh.h"

namespace CW::Core
{
	class CW_API Window
	{
	private:
		GLFWwindow* m_window;

	public:
		bool m_IsRunning;

	public:
		Window();
		~Window();

		bool InitWindow(const std::string name, const unsigned int height, const unsigned int width);
		void Update(); // ?? Should window be updated?
		void Terminate();
		void ProcessInput();
		void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	};

}