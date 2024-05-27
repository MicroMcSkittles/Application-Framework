#include "OpenGLRendererAPI.h"
#include "Renderer/Renderer.h"
#include <glad/glad.h>

namespace Engine::Renderer {
	namespace OpenGL {

		namespace {
			void SetWireFrame(bool v) {

			}
			void SetCull(bool v) {
				if (v) {
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					glFrontFace(GL_CCW);
				}
				else {
					glDisable(GL_CULL_FACE);
				}
			}
		}

		OpenGLRendererAPI::OpenGLRendererAPI() { }

		void OpenGLRendererAPI::Init()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}
		void OpenGLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			glViewport(x, y, width, height);
		}
		void OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}
		void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
			vertexArray->Unbind();
			Renderer::Renderer::GetDiagnostic().NumberDrawCalls += 1;
			Renderer::Renderer::GetDiagnostic().NumberTriangles += vertexArray->GetIndexBuffer()->GetCount() / 3;
		}
		void OpenGLRendererAPI::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGLRendererAPI::Enable(RenderFlag flag)
		{
			switch (flag)
			{
			case Engine::Renderer::RenderFlag::None: break;
			case Engine::Renderer::RenderFlag::WireFrame: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
			case Engine::Renderer::RenderFlag::TwoSided: SetCull(false); break;
			case Engine::Renderer::RenderFlag::DepthTest: glEnable(GL_DEPTH_TEST); break;
			}
		}
		void OpenGLRendererAPI::Disable(RenderFlag flag)
		{
			switch (flag)
			{
			case Engine::Renderer::RenderFlag::None: break;
			case Engine::Renderer::RenderFlag::WireFrame: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case Engine::Renderer::RenderFlag::TwoSided: SetCull(true); break;
			case Engine::Renderer::RenderFlag::DepthTest: glDisable(GL_DEPTH_TEST); break;
			}
		}
		void OpenGLRendererAPI::SetDepthFunc(DepthFunction func)
		{
			switch (func)
			{
			case Engine::Renderer::DepthFunction::Never: glDepthFunc(GL_NEVER); break;
			case Engine::Renderer::DepthFunction::Always: glDepthFunc(GL_ALWAYS); break;
			case Engine::Renderer::DepthFunction::Less: glDepthFunc(GL_LESS); break;
			case Engine::Renderer::DepthFunction::LessEqual: glDepthFunc(GL_LEQUAL); break;
			case Engine::Renderer::DepthFunction::Greater: glDepthFunc(GL_GREATER); break;
			case Engine::Renderer::DepthFunction::GreaterEqual: glDepthFunc(GL_GEQUAL); break;
			}
		}
	}
}