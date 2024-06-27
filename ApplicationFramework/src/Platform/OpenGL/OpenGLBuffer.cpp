#include "OpenGLBuffer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "core/Application.h"
#include "Renderer/RenderCommand.h"

namespace Engine::Renderer {
	namespace OpenGL {
		OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, uint32_t size)
		{
			glGenBuffers(1, &m_RenderID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		void OpenGLVertexBuffer::Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		}
		void OpenGLVertexBuffer::Unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const BufferLayout OpenGLVertexBuffer::GetLayout() const
		{
			return m_Layout;
		}

		void OpenGLVertexBuffer::SetLayout(const BufferLayout & layout)
		{
			m_Layout = layout;
		}


		OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size)
		{
			glGenBuffers(1, &m_RenderID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
			m_Count = size / sizeof(uint32_t);
		}

		void OpenGLIndexBuffer::Bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		}
		void OpenGLIndexBuffer::Unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		const uint32_t OpenGLIndexBuffer::GetCount() const
		{
			return m_Count;
		}

		OpenGLFrameBuffer::OpenGLFrameBuffer(bool UseColorBuffer, const std::vector<TextureProps>& TextureAttachments)
		{
			glGenFramebuffers(1, &m_RenderID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

			glm::vec2 wsize = { Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() };
			m_Size = wsize;
			std::vector<uint32_t> attachments;

			TextureProps depth_props;
			depth_props.DataType = TextureDataType::Float;
			depth_props.DataFormat = TextureFormat::DepthComponent;
			depth_props.InternalFormat = TextureFormat::DepthComponent;
			depth_props.MagFilter = TextureFilter::Nearest;
			depth_props.MinFilter = TextureFilter::Nearest;
			depth_props.SWrapMode = TextureWrapMode::Repeat;
			depth_props.TWrapMode = TextureWrapMode::Repeat;
			m_Textures.push_back(Texture2D::Create(wsize.x, wsize.y, depth_props));
			uint32_t depth_id = (uint32_t)m_Textures[0]->GetID();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0);
			attachments.push_back(GL_DEPTH_ATTACHMENT);

			if (!UseColorBuffer) {
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			for (uint32_t i = 0; i < TextureAttachments.size(); ++i) {
				m_Textures.push_back(Texture2D::Create(wsize.x, wsize.y, TextureAttachments[i]));
				uint32_t id = (uint32_t)m_Textures[i + 1]->GetID();
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0);
				attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			}

			if (attachments.size() > 0) {
				glDrawBuffers(attachments.size(), attachments.data());
			}

			/*if (m_DepthTesting) {
				glGenRenderbuffers(1, &m_RBO);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, wsize.x, wsize.y);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
			}*/

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				throw std::runtime_error("Failed to create frame buffer");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void OpenGLFrameBuffer::Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
		}
		void OpenGLFrameBuffer::Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
		{
			if (m_Size == glm::vec2(width, height)) return;
			glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
			m_Size = { width, height };

			TextureProps depth_props;
			depth_props.DataType = TextureDataType::Float;
			depth_props.DataFormat = TextureFormat::DepthComponent;
			depth_props.InternalFormat = TextureFormat::DepthComponent;
			depth_props.MagFilter = TextureFilter::Nearest;
			depth_props.MinFilter = TextureFilter::Nearest;
			depth_props.SWrapMode = TextureWrapMode::Repeat;
			depth_props.TWrapMode = TextureWrapMode::Repeat;
			m_Textures[0] = Texture2D::Create(m_Size.x, m_Size.y, depth_props);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (GLuint)m_Textures[0]->GetID(), 0);

			for (uint32_t i = 1; i < m_Textures.size(); ++i) {
				m_Textures[i] = Texture2D::Create(width, height, m_Textures[i]->GetProps());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (GLuint)m_Textures[i]->GetID(), 0);
			}


			//RenderCommand::SetViewport(0, 0, width, height);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Shader Storage Buffer
		OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t binding, uint32_t size)
			:m_Binding(binding), m_Size(size)
		{
			glGenBuffers(1, &m_RenderID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, NULL, GL_DYNAMIC_READ);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void OpenGLShaderStorageBuffer::Bind() const
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RenderID);
		}
		void OpenGLShaderStorageBuffer::Unbind() const
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, 0);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void OpenGLShaderStorageBuffer::resize(uint32_t size)
		{
			m_Size = size;
			glGenBuffers(1, &m_RenderID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, NULL, GL_DYNAMIC_READ);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
		void OpenGLShaderStorageBuffer::subData(uint32_t offset, uint32_t size, const void * data)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
}
}