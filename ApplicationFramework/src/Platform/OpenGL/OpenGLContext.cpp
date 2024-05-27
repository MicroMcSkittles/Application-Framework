#include "OpenGLContext.h"
#include "core/Logger.h"
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine::Renderer {
	namespace OpenGL {
		OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
			: m_WindowHandle(windowHandle)
		{
			if (!windowHandle) {
				std::cout << "Window handle is NULL\n";
			}
		}
		void OpenGLContext::Init()
		{
			glfwMakeContextCurrent(m_WindowHandle);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (!status) {
				std::cout << "Failed to initialize Glad\n";
			}
			Logger::LogInfo("OpenGL Info:\n\t" + std::string((const char*)glGetString(GL_VENDOR)) + "\n\t" + std::string((const char*)glGetString(GL_RENDERER)) + "\n\t" + std::string((const char*)glGetString(GL_VERSION)));
		}
		void OpenGLContext::SwapBuffers()
		{
			glfwSwapBuffers(m_WindowHandle);
		}
	}
}