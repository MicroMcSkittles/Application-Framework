#include <Engine.h>
#include <core/EntryPoint.h>

#include <Renderer/CameraController.h>

#include <AssetLoading/OBJLoader.h>

#include <iostream>

using namespace Engine;

namespace {
	std::vector<float> skyboxVertices = {
		-3,  3, -3,
		 3,  3, -3,
		-3, -3, -3,
		 3, -3, -3,
		-3,  3,  3,
		 3,  3,  3,
		-3, -3,  3,
		 3, -3,  3
	};
	std::vector<uint32_t> skyboxIndices = {
		0, 1, 2, // Side 0
		2, 1, 3,
		4, 0, 6, // Side 1
		6, 0, 2,
		7, 5, 6, // Side 2
		6, 5, 4,
		3, 1, 7, // Side 3 
		7, 1, 5,
		4, 5, 0, // Side 4 
		0, 5, 1,
		3, 7, 2, // Side 5 
		2, 7, 6
	};

	std::vector<Renderer::TextureProps> CubeMapProps = {
		{"Assets/Textures/right.jpg"},
		{"Assets/Textures/left.jpg"},
		{"Assets/Textures/top.jpg"},
		{"Assets/Textures/bottom.jpg"},
		{"Assets/Textures/front.jpg"},
		{"Assets/Textures/back.jpg"}
	};
}

class MainLayer : public Layer {
public:
	MainLayer() : Layer("Main Layer") { }
	~MainLayer() { }

	virtual void onAttach() override { 
		m_MainCamera = Renderer::PerspectiveCamera::Create(
			Application::Get().GetWindow().GetWidth(),
			Application::Get().GetWindow().GetHeight(),
			45.0f, 0.1f, 5000.0f
		);
		m_MainCamera->setDirection({ 0,0,-1 });
		m_CameraController = Renderer::CameraController::Create(m_MainCamera, 500);

		m_PostProcShader = Renderer::Shader::Create(
			"assets/Shaders/Effects/PostProcessShader.vert",
			"assets/Shaders/Effects/PostProcessShader.frag"
		);
		m_SkyBoxShader = Renderer::Shader::Create(
			"assets/Shaders/Effects/SkyBoxShader.vert",
			"assets/Shaders/Effects/SkyBoxShader.frag"
		);
		m_MainShader = Renderer::Shader::Create(
			"assets/Shaders/Phong.vert",
			"assets/Shaders/Phong.frag"
		);

		m_SkyBoxMesh = Renderer::Mesh::Create(skyboxVertices, skyboxIndices);
		CubeMapProps[0].TWrapMode = Renderer::TextureWrapMode::ClampToEdge;
		CubeMapProps[0].SWrapMode = Renderer::TextureWrapMode::ClampToEdge;
		CubeMapProps[0].RWrapMode = Renderer::TextureWrapMode::ClampToEdge;
		m_SkyBoxCubeMap = Renderer::CubeMap::Create(CubeMapProps, false);

		m_MainModel = AssetLoader::OBJLoader::Load("assets/Models/sponza.obj");
	}

	virtual void onUpdate(float delta_time) override { 
		m_CameraController->OnUpdate(delta_time);

		if (Input::IsKeyPressed(KEY_TAB)) {
			m_PostProcShader = Renderer::Shader::Create(
				"assets/Shaders/Effects/PostProcessShader.vert",
				"assets/Shaders/Effects/PostProcessShader.frag"
			);
			m_SkyBoxShader = Renderer::Shader::Create(
				"assets/Shaders/Effects/SkyBoxShader.vert",
				"assets/Shaders/Effects/SkyBoxShader.frag"
			);
			m_MainShader = Renderer::Shader::Create(
				"assets/Shaders/Phong.vert",
				"assets/Shaders/Phong.frag"
			);
		}
	}

	virtual void onRender() override { 
		Renderer::Renderer::BeginFrame(m_MainCamera);
		// Skybox
		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetUniform("View", Renderer::UDMat4::Create(glm::mat4(glm::mat3(m_MainCamera->getView()))));
		m_SkyBoxShader->SetUniform("Projection", Renderer::UDMat4::Create(m_MainCamera->getProjection()));
		m_SkyBoxShader->SetUniform("skybox", Renderer::UDTexture::Create(m_SkyBoxCubeMap));
		m_SkyBoxShader->Unbind();

		Renderer::RenderCommand::SetDepthFunc(Renderer::DepthFunction::LessEqual);
		Renderer::RenderCommand::Enable(Renderer::RenderFlag::TwoSided);

		Renderer::Renderer::Submit(m_SkyBoxMesh, glm::mat4(1), m_SkyBoxShader);

		Renderer::RenderCommand::Disable(Renderer::RenderFlag::TwoSided);
		Renderer::RenderCommand::SetDepthFunc(Renderer::DepthFunction::Less);
		// ======

		Renderer::Renderer::Submit(m_MainModel, glm::mat4(1), m_MainShader);

		Renderer::Renderer::EndFrame();

		Renderer::Renderer::Render(m_PostProcShader);
	}

	virtual void onImGuiRender() override { 
	
	}

private:
	std::shared_ptr<Renderer::Camera> m_MainCamera;
	std::shared_ptr<Renderer::CameraController> m_CameraController;
	std::shared_ptr<Renderer::Shader> m_PostProcShader;

	std::shared_ptr<Renderer::Shader> m_SkyBoxShader;
	std::shared_ptr<Renderer::Mesh> m_SkyBoxMesh;
	std::shared_ptr<Renderer::CubeMap> m_SkyBoxCubeMap;

	std::shared_ptr<Renderer::Shader> m_MainShader;
	std::shared_ptr<Renderer::Model> m_MainModel;
};

class App : public Application {
public:
	App(int argc, char** argv) : Application(argc, argv) {
		pushLayer(new MainLayer());
	}
	~App() { }
};

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
	return new App(argc, argv);
}
