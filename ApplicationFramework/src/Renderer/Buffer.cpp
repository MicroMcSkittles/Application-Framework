#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine::Renderer {
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float * vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<VertexBuffer>(new OpenGL::OpenGLVertexBuffer(vertices, size));
		}
		return std::shared_ptr<VertexBuffer>();
	}
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<IndexBuffer>(new OpenGL::OpenGLIndexBuffer(indices, size));
		}
		return std::shared_ptr<IndexBuffer>();
	}
	std::shared_ptr<FrameBuffer> FrameBuffer::Create(bool DepthTest, const std::vector<TextureProps>& TextureAttachments)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<FrameBuffer>(new OpenGL::OpenGLFrameBuffer(DepthTest, TextureAttachments));
		}
		return std::shared_ptr<FrameBuffer>();
	}
	std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::Create(uint32_t binding, uint32_t elementSize, uint32_t count)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL: return std::shared_ptr<ShaderStorageBuffer>(new OpenGL::OpenGLShaderStorageBuffer(binding, elementSize, count));
		}
		return std::shared_ptr<ShaderStorageBuffer>();
	}
}