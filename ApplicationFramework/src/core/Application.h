#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
//#include "Engine/ImGui/ImGuiLayer.h"

namespace Engine {
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
		Application(int argc, char** argv);
		// Application deconstructor.
		virtual ~Application();

		// Starts the main loop.
		void run();

		// Adds a Layer to the application.
		void pushLayer(Layer* layer);
		// Adds a Layer to the application that gets displayed over all other layers.
		void pushOverlay(Layer* layer);

		// Returns a reference to the window.
		//inline Window& GetWindow() { return *m_Window; }
		// Returns a reference to the ImGui Layer if enabled.
		//inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }
	    // Returns a reference to the Application instace.
		inline static Application& Get() { return *s_Instance; }

		// Returns the argument count.
		inline int GetArgC() { return m_Argc; }
		// Returns a list of arguments.
		inline char** GetArgV() { return m_Argv; }

		// Returns basic diagnostic information.
		inline DiagnosticInfo& GetDiagnostic() { return m_DiagnosticInfo; }

		// Displays ImGui window containing diagnostic information if ImGui is enabled.
		void ShowDiagnostic();
		// Processes a event reference passed in as e
		void ProcEvent(Event& e);

	private:
		// On Window Close event function.
		bool OnWindowClose(WindowCloseEvent& e);
		// On Window Resize event function.
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		// Main application window.
		//std::unique_ptr<Window> m_Window;
		// A list of all registered layers.
		LayerStack m_LayerStack;

		// A pointer to the ImGui layer.
		//ImGuiLayer* m_ImGuiLayer;

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

	private:
		// Application Instance.
		inline static Application* s_Instance;
	};

	// A required user defined function used to create user defined applications.
	Application* CreateApplication(int argc, char** argv);
}