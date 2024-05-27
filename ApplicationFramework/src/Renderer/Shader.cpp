#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine::Renderer {
	std::shared_ptr<Shader> Shader::Create(std::string vertexPath, std::string fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return std::shared_ptr<Shader>(new OpenGL::OpenGLShader(vertexPath, fragmentPath));
		}
		return std::shared_ptr<Shader>();
	}
	std::shared_ptr<Shader> Shader::Create(std::string vertexPath, std::string geometryPath, std::string fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return std::shared_ptr<Shader>(new OpenGL::OpenGLShader(vertexPath, geometryPath, fragmentPath));
		}
		return std::shared_ptr<Shader>();
	}

	std::shared_ptr<UniformList> UniformList::Create(int binding, uint32_t elmSize, int count, const void* data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return std::shared_ptr<UniformList>(new OpenGL::OpenGLUniformList(binding, elmSize, count, data));
		}
		return std::shared_ptr<UniformList>();
	}

	std::shared_ptr<UDLTexture> UDLTexture::Create(const std::vector<std::shared_ptr<Texture>>& value) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return std::shared_ptr<UDLTexture>(new OpenGL::OpenGLUDLTexture(value));
		}
		return std::shared_ptr<UDLTexture>();
	}
}