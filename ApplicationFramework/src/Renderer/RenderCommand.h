#pragma once
#include <glm/glm.hpp>
#include "core/Core.h"
#include "VertexArray.h"
#include "RendererAPI.h"

namespace Engine::Renderer {
	class RenderCommand
	{
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}
		
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& VAO) {
			s_RendererAPI->DrawIndexed(VAO);
		}

		inline static void Enable(RenderFlag flag) {
			s_RendererAPI->Enable(flag);
		}
		inline static void Disable(RenderFlag flag) {
			s_RendererAPI->Disable(flag);
		}

		inline static void SetDepthFunc(DepthFunction func) {
			s_RendererAPI->SetDepthFunc(func);
		}

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};
}