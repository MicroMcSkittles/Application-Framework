#pragma once
#include "core/Core.h"
#include "core/Event/Event.h"


namespace Engine {
	struct WindowProps
	{
		std::string Title;
		unsigned int width;
		unsigned int height;

		WindowProps(std::string Title = "Window", unsigned int width = 1024, unsigned int height = 720)
			:Title(Title), width(width), height(height) { }
	};

	class Window {
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;

		virtual void SetEventCallback(const EventCallBackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}