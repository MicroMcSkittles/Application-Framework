#include "Application.h"
#include "Renderer/Renderer.h"
#include "core/System.h"
#include <imgui.h>
#include <iostream>

namespace Engine {
	Application::Application(int argc, char** argv, uint32_t PropFlags)
	{
		m_Flags = PropFlags;
		m_Argc = argc;
		m_Argv = argv;
		if (s_Instance) {
			std::cout << "There is allready a instance of application\n";
			return;
		}
		s_Instance = this;


		if(m_Flags & AppProp_Window_Enabled) {
			m_Window = Window::Create();
			m_Window->SetEventCallback(BIND_EVENT_FN(Application::ProcessEvent));
		}

		if (m_Flags & AppProp_Renderer_Enabled) {
			Renderer::Renderer::Init();
		}

		if (m_Flags & AppProp_ImGui_Enabled) {
			m_ImGuiLayer = ImGuiLayer::Create();
			PushOverlay(m_ImGuiLayer);
		}

		m_Running = true;
		last_frame_time = System::GetTime();
	}
	Application::~Application()
	{

	}

	void Application::PushLayer(std::shared_ptr<Layer> layer)
	{
		m_PushCommands.push_back({
			layer,
			false
		});
	}
	void Application::PushOverlay(std::shared_ptr<Layer> layer)
	{
		m_PushCommands.push_back({
			layer,
			true
		});
	}
	void Application::PopLayer(std::shared_ptr<Layer> layer)
	{
		m_PopCommands.push_back({
			layer,
			false
		});
	}
	void Application::PopOverlay(std::shared_ptr<Layer> layer)
	{
		m_PopCommands.push_back({
			layer,
			true
		});
	}

	void Application::ShowDiagnostic()
	{
		ImGui::Begin("Application Diagnostic");
		ImGui::Text("FPS: %f", m_DiagnosticInfo.FPS);
		ImGui::Text("MS: %f", m_DiagnosticInfo.MS);
		ImGui::Text("Game Update MS: %f", m_DiagnosticInfo.UpdateMS);
		ImGui::Text("ImGui MS: % f", m_DiagnosticInfo.ImGuiMS);
		ImGui::Text("Renderer MS: %f", m_DiagnosticInfo.RendererMS);
		ImGui::End();
	}

	void Application::ProcessEvent(Event & e)
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

	void Application::Run()
	{
		while (m_Running) {
			float time = System::GetTime();
			float delta_time = time - last_frame_time;
			last_frame_time = time;
			m_DiagnosticInfo.MS = delta_time * 1000;
			m_DiagnosticInfo.FPS = 1 / delta_time;

			ExacuteLayerCommands();

			time = System::GetTime();
			for (std::shared_ptr<Layer> layer : m_LayerStack) {
				layer->onUpdate(delta_time);
			}
			m_DiagnosticInfo.UpdateMS = (System::GetTime() - time) * 1000;

			if (m_Flags & AppProp_Renderer_Enabled) {
				time = System::GetTime();
				for (std::shared_ptr<Layer> layer : m_LayerStack) {
					layer->onRender();
				}
				m_DiagnosticInfo.RendererMS = (System::GetTime() - time) * 1000;
				Renderer::Renderer::GetDiagnostic().Milliseconds = m_DiagnosticInfo.RendererMS;
			}

			if (m_Flags & AppProp_ImGui_Enabled) {
				time = System::GetTime();
				m_ImGuiLayer->Begin();
				for (std::shared_ptr<Layer> layer : m_LayerStack) {
					layer->onImGuiRender();
				}
				m_ImGuiLayer->End();
				m_DiagnosticInfo.ImGuiMS = (System::GetTime() - time) * 1000;
			}

			if (m_Flags & AppProp_Window_Enabled) m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent & e)
	{
		m_Running = false;
		return false;
	}
	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		Renderer::Renderer::OnWindowResize(e.getWidth(), e.getHeight());
		return false;
	}
	void Application::ExacuteLayerCommands()
	{
		if (!m_PopCommands.size() && !m_PushCommands.size()) return;
		for (auto& command : m_PopCommands) {
			if (command.isOverlay) {
				m_LayerStack.PopOverlay(command.layer);
			}
			else {
				m_LayerStack.PopLayer(command.layer);
			}
			command.layer->onDetach();
		}
		m_PopCommands.clear();

		for (auto& command : m_PushCommands) {
			if (command.isOverlay) {
				m_LayerStack.PushOverlay(command.layer);
			}
			else {
				m_LayerStack.PushLayer(command.layer);
			}
			command.layer->onAttach();
		}
		m_PushCommands.clear();
	}
}