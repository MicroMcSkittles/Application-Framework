#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine::Renderer {
	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = std::unique_ptr<RendererAPI>(new OpenGL::OpenGLRendererAPI);
}