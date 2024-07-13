#include <Engine.h>
#include <core/EntryPoint.h>

#include <Renderer/CameraController.h>

#include <AssetLoading/OBJLoader.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;

class MainLayer : public Layer {
public:
	~MainLayer() { }

	static std::shared_ptr<MainLayer> Create() {
		return std::shared_ptr<MainLayer>(new MainLayer());
	}

	virtual void onAttach() override { 
		m_MainCamera = Renderer::OrthographicCamera::Create(
			Application::Get().GetWindow().GetWidth(),
			Application::Get().GetWindow().GetHeight(),
			-1000, 1000, 5
		);
		m_MainCamera->setDirection({ 0,0,-1 });
		m_MainCamera->setPosition({ 0,0,-1 });

		m_CameraController = Renderer::CameraController2D::Create(m_MainCamera, 5);

		m_PostProcShader = Renderer::Shader::Create(
			"assets/Shaders/Effects/PostProcessShader.vert",
			"assets/Shaders/Effects/PostProcessShader.frag"
		);
		m_MainShader = Renderer::Shader::Create(
			"assets/Shaders/Phong.vert",
			"assets/Shaders/Effects/SolidColor.frag"
		);
	}

	virtual void onDetach() override {
		std::cout << "Detached\n";
	}

	virtual void onUpdate(float delta_time) override { 
		m_CameraController->OnUpdate(delta_time);

		if (Input::IsKeyPressed(KEY_TAB)) {
			m_PostProcShader = Renderer::Shader::Create(
				"assets/Shaders/Effects/PostProcessShader.vert",
				"assets/Shaders/Effects/PostProcessShader.frag"
			);
			m_MainShader = Renderer::Shader::Create(
				"assets/Shaders/Phong.vert",
				"assets/Shaders/Effects/SolidColor.frag"
			);
		}
	}

	virtual void onRender() override { 
		Renderer::Renderer2D::BeginFrame(m_MainCamera);

		Renderer::Renderer2D::DrawQuad({ 2,0 }, { 1,1 }, { 1,1,1 }, { 1,1,1 }, m_MainShader);
		Renderer::Renderer2D::DrawRotatedQuad({ -2,2 }, { 1,1 }, glm::radians(45.0f), { 1,1,1 }, { 1,1,1 }, m_MainShader);

		Renderer::Renderer2D::EndFrame();

		Renderer::Renderer2D::Render(m_PostProcShader);
	}

	virtual void onImGuiRender() override { 
		Application::Get().ShowDiagnostic();
	}

private:
	MainLayer() : Layer("Main Layer") { }

private:
	std::shared_ptr<Renderer::Camera> m_MainCamera;
	std::shared_ptr<Renderer::CameraController2D> m_CameraController;
	std::shared_ptr<Renderer::Shader> m_PostProcShader;

	std::shared_ptr<Renderer::Shader> m_MainShader;
};

class App : public Application {
public:
	App(int argc, char** argv) : Application(argc, argv) {
		std::shared_ptr<MainLayer> layer = MainLayer::Create();
		PushLayer(layer);
	}
	~App() { }
};

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
	return new App(argc, argv);
}
