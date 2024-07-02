#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine {
	enum ApplicationProp
	{
		AppProp_None = 0,
		AppProp_ImGui_Enabled,
		AppProp_Renderer_Enabled,
		AppProp_Window_Enabled
	};
	
	// Properties applied to the application before it starts
	constexpr uint32_t DefaultPropFlags = AppProp_ImGui_Enabled | AppProp_Renderer_Enabled | AppProp_Window_Enabled;
	
	// Basic information for diagnosing problems with the framework.
	struct DiagnosticInfo {
		// Frames Per Second.
		float FPS;
		// The number of milliseconds used to complete last frame.
		float MS;
		// The number of milliseconds the renderer used last frame.
		float RendererMS;
		// The number of milliseconds ImGui used last frame.
		float ImGuiMS;
		// The number of milliseconds used to update last frame.
		float UpdateMS;
	};

	// Application base class. It handels the window, layers, ImGui, and events.
	class Application
	{
	public:
		// Application constructor. It takes in the argument count and the list of arguments.
		Application(int argc, char** argv, uint32_t PropFlags = DefaultPropFlags);
		// Application deconstructor.
		virtual ~Application();

		// Starts the main loop.
		void Run();

		// Adds a Layer to the application.
		void PushLayer(std::shared_ptr<Layer> layer);
		// Adds a Layer to the application that gets displayed over all other layers.
		void PushOverlay(std::shared_ptr<Layer> layer);
		// Removes a Layer from the application.
		void PopLayer(std::shared_ptr<Layer> layer);
		// Removes a Overlay from the application.
		void PopOverlay(std::shared_ptr<Layer> layer);

		// Returns a reference to the window.
		inline Window& GetWindow() { return *m_Window; }
		// Returns a reference to the ImGui Layer if enabled.
		inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }
	    // Returns a reference to the Application instace.
		inline static Application& Get() { return *s_Instance; }

		// Returns the argument count.
		inline int GetArgumentCount() { return m_Argc; }
		// Returns a list of arguments.
		inline char** GetArgumentValues() { return m_Argv; }
		// Returns properties
		inline uint32_t GetFlags() { return m_Flags; }

		// Returns basic diagnostic information.
		inline DiagnosticInfo& GetDiagnostic() { return m_DiagnosticInfo; }

		// Displays ImGui window containing diagnostic information if ImGui is enabled.
		void ShowDiagnostic();
		// Processes a event reference passed in as e
		void ProcessEvent(Event& e);

	private:
		// On Window Close event function.
		bool OnWindowClose(WindowCloseEvent& e);
		// On Window Resize event function.
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		// Main application window.
		std::unique_ptr<Window> m_Window;
		// A list of all registered layers.
		LayerStack m_LayerStack;

		// A pointer to the ImGui layer.
		std::shared_ptr<ImGuiLayer> m_ImGuiLayer;

		// A flag storing if the application is running.
		bool m_Running;
		// Used to calculate delta time.
		float last_frame_time;

		// Application argument count.
		int m_Argc;
		// Application arguments.
		char** m_Argv;

		// Application Diagnostic Information.
		DiagnosticInfo m_DiagnosticInfo;

		uint32_t m_Flags;

	private:
		// Application Instance.
		inline static Application* s_Instance;
	};

	// A required user defined function used to create user defined applications.
	Application* CreateApplication(int argc, char** argv);
}