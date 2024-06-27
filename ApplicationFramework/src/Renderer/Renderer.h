#pragma once
#include "RenderCommand.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"


namespace Engine::Renderer {
	struct RendererDiagnosticInfo {
		float Milliseconds;
		int NumberDrawCalls;
		int NumberUniformCalls;
		int NumberTriangles;
	};

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(unsigned int width, unsigned int height);

		static void BeginFrame(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Light>>& Lights);
		static void EndFrame();
		static void Render(std::shared_ptr<Shader> postProcShader);
		static std::shared_ptr<Texture> GetRender();

		static void Submit(std::shared_ptr<Model> model, const glm::mat4& transform, std::shared_ptr<Shader> shader);
		static void Submit(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<Shader> shader);
		static void Submit(std::shared_ptr<VertexArray> VAO, const glm::mat4& transform, std::shared_ptr<Shader> shader);

		static void ShowRendererDiagnostic();
		static RendererDiagnosticInfo& GetDiagnostic() { return m_DiagnosticInfo; }

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct LightData {
			std::shared_ptr<ShaderStorageBuffer> m_PointLightStorageBuffer;
			std::shared_ptr<ShaderStorageBuffer> m_DirectionalLightStorageBuffer;
			std::shared_ptr<ShaderStorageBuffer> m_SpotLightStorageBuffer;
			uint32_t LFPointLightCount;
			uint32_t LFDirectionalLightCount;
			uint32_t LFSpotLightCount;
		};
		struct SceneData {
			uint32_t m_Width;
			uint32_t m_Height;
			std::shared_ptr<Camera> m_Camera;
			std::shared_ptr<FrameBuffer> m_PostProcFrameBuffer;
			std::shared_ptr<ShaderStorageBuffer> m_GeneralStorageBuffer;
			LightData m_LightData;
			uint32_t PositionBufferIndex;
			uint32_t NormalBufferIndex;
			uint32_t ColorSpecBufferIndex;
		};
		inline static RendererDiagnosticInfo m_DiagnosticInfo;
		inline static SceneData m_Data;
	};
}