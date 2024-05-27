#pragma once
#include "Renderer/RendererAPI.h"
#include <glm/glm.hpp>

namespace Engine::Renderer {
	namespace OpenGL {
		class OpenGLRendererAPI : public RendererAPI
		{
		public:
			OpenGLRendererAPI();

			virtual void Init() override;
			virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
			virtual void SetClearColor(const glm::vec4 & color) override;
			virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
			virtual void Enable(RenderFlag flag) override;
			virtual void Disable(RenderFlag flag) override;
			virtual void SetDepthFunc(DepthFunction func) override;
			virtual void Clear() override;
		};
	}
}