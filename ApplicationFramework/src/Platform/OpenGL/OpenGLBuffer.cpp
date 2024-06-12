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

		OpenGLFrameBuffer::OpenGLFrameBuffer(bool DepthTest, const std::vector<TextureProps>& TextureAttachments)
		{
			m_DepthTesting = DepthTest;

			glGenFramebuffers(1, &m_RenderID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

			glm::vec2 wsize = { Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() };
			m_Size = wsize;
			std::vector<uint32_t> attachments;

			for (uint32_t i = 0; i < TextureAttachments.size(); ++i) {
				m_Textures.push_back(Texture2D::Create(wsize.x, wsize.y, TextureAttachments[i]));
				uint32_t id = (uint32_t)m_Textures[i]->GetID();
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0);
				attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			}

			if (attachments.size() > 0) {
				glDrawBuffers(attachments.size(), attachments.data());
			}

			if (m_DepthTesting) {
				glGenRenderbuffers(1, &m_RBO);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, wsize.x, wsize.y);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
			}

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

			for (uint32_t i = 0; i < m_Textures.size(); ++i) {
				m_Textures[i] = Texture2D::Create(width, height, m_Textures[i]->GetProps());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (GLuint)m_Textures[i]->GetID(), 0);
			}

			if (m_DepthTesting) {
				glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
			}

			//RenderCommand::SetViewport(0, 0, width, height);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}