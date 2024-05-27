#pragma once
#include "core/Core.h"
#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Engine::Renderer {
	enum class RenderFlag {
		None,
		WireFrame,
		TwoSided,
		DepthTest
	};

	enum class DepthFunction {
		Never,
		Always,
		Less,
		LessEqual,
		Greater,
		GreaterEqual
	};

	class RendererAPI
	{
	public:
		enum class API {
			None = 0, OpenGL = 1
		};
	public:

		virtual void Init() = 0;

		virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Enable(RenderFlag flag) = 0;
		virtual void Disable(RenderFlag flag) = 0;

		virtual void SetDepthFunc(DepthFunction func) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}