#include "Application.h"
//#include "Engine/Renderer/Renderer.h"
#include "core/System.h"
#include <iostream>
//#include <imGUI/imgui.h>

namespace Engine {
	Application::Application(int argc, char** argv)
	{
		m_Argc = argc;
		m_Argv = argv;
		if (s_Instance) {
			std::cout << "There is allready a instance of application\n";
			return;
		}
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::ProcEvent));

		//Renderer::Renderer::Init();
		
		m_Running = true;

		//m_ImGuiLayer = new ImGuiLayer();
		//pushOverlay(m_ImGuiLayer);

		//last_frame_time = System::GetTime();
	}
	Application::~Application()
	{

	}

	void Application::pushLayer(Layer * layer)
	{
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}
	void Application::pushOverlay(Layer * layer)
	{
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::ShowDiagnostic()
	{
		//ImGui::Begin("Application Diagnostic");
		//ImGui::Text("FPS: %f", m_DiagnosticInfo.FPS);
		//ImGui::Text("MS: %f", m_DiagnosticInfo.MS);
		//ImGui::Text("Game Update MS: %f", m_DiagnosticInfo.UpdateMS);
		//ImGui::Text("ImGui MS: % f", m_DiagnosticInfo.ImGuiMS);
		//ImGui::Text("Renderer MS: %f", m_DiagnosticInfo.RendererMS);
		//ImGui::End();
	}

	void Application::ProcEvent(Event & e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		eventDispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e); 
			if (e.m_Handled) {
				break;
			}
		}
	}

	void Application::run()
	{
		while (m_Running) {

			float time = System::GetTime();
			float delta_time = time - last_frame_time;
			last_frame_time = time;
			m_DiagnosticInfo.MS = delta_time * 1000;
			m_DiagnosticInfo.FPS = 1 / delta_time;

			time = System::GetTime();
			for (Layer* layer : m_LayerStack) {
				layer->onUpdate(delta_time);
			}
			m_DiagnosticInfo.UpdateMS = (System::GetTime() - time) * 1000;

			time = System::GetTime();
			for (Layer* layer : m_LayerStack) {
				layer->onRender();
			}
			m_DiagnosticInfo.RendererMS = (System::GetTime() - time) * 1000;
			//Renderer::Renderer::GetDiagnostic().Milliseconds = m_DiagnosticInfo.RendererMS;

			time = System::GetTime();
			//m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->onImGuiRender();
			}
			//m_ImGuiLayer->End();
			m_DiagnosticInfo.ImGuiMS = (System::GetTime() - time) * 1000;

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent & e)
	{
		m_Running = false;
		return false;
	}
	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		//Renderer::Renderer::OnWindowResize(e.getWidth(), e.getHeight());
		return false;
	}
}