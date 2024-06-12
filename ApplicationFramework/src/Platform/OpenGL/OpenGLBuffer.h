#pragma once
#include "Renderer/Buffer.h"

namespace Engine::Renderer {
	namespace OpenGL {
		class OpenGLVertexBuffer : public VertexBuffer {
		public:
			OpenGLVertexBuffer(float* vertices, uint32_t size);

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const BufferLayout GetLayout() const override;
			virtual void SetLayout(const BufferLayout& layout) override;

		private:
			uint32_t m_RenderID;
			BufferLayout m_Layout;
		};

		class OpenGLIndexBuffer : public IndexBuffer {
		public:
			OpenGLIndexBuffer(uint32_t* indices, uint32_t size);

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const uint32_t GetCount() const override;

		private:
			uint32_t m_RenderID;
			uint32_t m_Count;
		};

		class OpenGLFrameBuffer : public FrameBuffer {
		public:
			OpenGLFrameBuffer(bool DepthTest, const std::vector<TextureProps>& TextureAttachments);

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual void Resize(uint32_t width, uint32_t height) override;

			virtual std::shared_ptr<Texture2D> GetTexture(int index) const override { return m_Textures[index]; }

			virtual glm::vec2 GetSize() const override { return m_Size; }

		private:
			uint32_t m_RenderID, m_RBO;
			bool m_DepthTesting;
			glm::vec2 m_Size;
			std::vector<std::shared_ptr<Texture2D>> m_Textures;
		};
	}
}
