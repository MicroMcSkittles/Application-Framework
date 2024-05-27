#include "Texture.h"
#include "TextureHandler.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine::Renderer {
	std::shared_ptr<Texture2D> Texture2D::Create(const TextureProps & props, bool CheckHandler)
	{
		if (TextureHandler::HasTexture(props.Path) && CheckHandler) return std::dynamic_pointer_cast<Texture2D>(TextureHandler::GetTexture(props.Path));

		std::shared_ptr<Texture2D> texture;

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: texture = std::shared_ptr<Texture2D>(new OpenGL::OpenGLTexture2D(props.Path, props));
		}

		if (TextureHandler::HasTexture(props.Path)) TextureHandler::SetTexture(props.Path, texture);
		else  TextureHandler::RegTexture(texture, props.Path);
		return texture;
	}
	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const TextureProps & props)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<Texture2D>(new OpenGL::OpenGLTexture2D(width, height, props));
		}
		return std::shared_ptr<Texture2D>();
	}

	std::shared_ptr<CubeMap> CubeMap::Create(const std::vector<TextureProps>& props, bool CheckHandler)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<CubeMap>(new OpenGL::OpenGLCubeMap(props));
		}
		return std::shared_ptr<CubeMap>();
	}

}