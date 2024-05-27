#pragma once
#include "core/Window.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Engine::Platform::Windows {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;
		virtual float GetAspectRatio() const override;

		virtual void SetEventCallback(const EventCallBackFn & callback) override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void * GetNativeWindow() const override;
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<Renderer::OpenGL::OpenGLContext> m_Context;

		struct WindowData {
			std::string Title;
			unsigned int width;
			unsigned int height;
			
			bool vsync;

			EventCallBackFn callback;
		};

		WindowData m_Data;
	};
}