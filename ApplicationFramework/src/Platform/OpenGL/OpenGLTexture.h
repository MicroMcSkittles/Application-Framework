#pragma once
#include "Renderer/Texture.h"

namespace Engine::Renderer {
	namespace OpenGL {
		class OpenGLTexture2D : public Texture2D {
		public:
			OpenGLTexture2D(std::string path, const TextureProps& props);
			OpenGLTexture2D(uint32_t width, uint32_t height,const TextureProps& props);
			~OpenGLTexture2D();

			virtual void Resize(uint32_t width, uint32_t height) override;

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual void* GetID() const override { 
				return (void*)m_RendererID; }

			uint64_t GetHandle() const { return m_Handle; }
			virtual glm::vec2 GetSize() const override { return m_Size; }
			virtual TextureProps GetProps() const override { return m_Props; }
		private:
			glm::vec2 m_Size;
			TextureProps m_Props;

			uint32_t m_RendererID;
			uint64_t m_Handle;
		};

		class OpenGLCubeMap : public CubeMap {
		public:
			OpenGLCubeMap(const std::vector<TextureProps>& props);
			~OpenGLCubeMap();

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual void* GetID() const { return (void*)m_RendererID; }

			virtual std::vector<TextureProps>& GetProps() { return m_Props; }

			uint64_t GetHandle() const { return m_Handle; }

		private:
			std::vector<TextureProps> m_Props;

			uint32_t m_RendererID;
			uint64_t m_Handle;
		};
	}
}