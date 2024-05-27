#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine::Renderer {
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<VertexArray>(new OpenGL::OpenGLVertexArray());
		}
		return std::shared_ptr<VertexArray>();
	}
}