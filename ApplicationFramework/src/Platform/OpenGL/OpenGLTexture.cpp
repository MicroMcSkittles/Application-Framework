#include "OpenGLTexture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "core/Logger.h"

namespace Engine::Renderer {
	namespace OpenGL {
		namespace {
			uint32_t TextureWrapModetoOpenGL(TextureWrapMode m) {
				switch (m)
				{
				case Engine::Renderer::TextureWrapMode::None: return 0;
				case Engine::Renderer::TextureWrapMode::Repeat: return GL_REPEAT;
				case Engine::Renderer::TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
				case Engine::Renderer::TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
				case Engine::Renderer::TextureWrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
				}
			}
			uint32_t TextureFormattoOpenGL(TextureFormat m) {
				switch (m)
				{
				case Engine::Renderer::TextureFormat::Monocrome: return GL_RED;
				case Engine::Renderer::TextureFormat::R8: return GL_R8;
				case Engine::Renderer::TextureFormat::RG: return GL_RG;
				case Engine::Renderer::TextureFormat::RG8: return GL_RG8;
				case Engine::Renderer::TextureFormat::RGB: return GL_RGB;
				case Engine::Renderer::TextureFormat::RGB8: return GL_RGB8;
				case Engine::Renderer::TextureFormat::RGB16Float: return GL_RGB16F;
				case Engine::Renderer::TextureFormat::RGBA: return GL_RGBA;
				}
			}
			uint32_t TextureFiltertoOpenGL(TextureFilter m) {
				switch (m)
				{
				case Engine::Renderer::TextureFilter::Nearest: return GL_NEAREST;
				case Engine::Renderer::TextureFilter::Linear: return GL_LINEAR;
				}
			}
			uint32_t TextureDataTypetoOpenGL(TextureDataType m) {
				switch (m)
				{
				case Engine::Renderer::TextureDataType::Float: return GL_FLOAT;
				case Engine::Renderer::TextureDataType::Ubyte: return GL_UNSIGNED_BYTE;
				}
			}
		}

		OpenGLTexture2D::OpenGLTexture2D(std::string path, const TextureProps & props)
		{
			m_Props = props;
			stbi_set_flip_vertically_on_load(!m_Props.FlipY);

			
			//// load and generate the texture
			int width, height, nrChannels;
			unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum internalFormat = 0, dataFormat = 0;
				if (nrChannels == 4) {
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
				}
				else if (nrChannels == 3) {
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
				}
				else if (nrChannels == 2) {
					internalFormat = GL_RG8;
					dataFormat = GL_RG;
				}
				else if (nrChannels == 1) {
					internalFormat = GL_R8;
					dataFormat = GL_RED;
				}
				glGenTextures(1, &m_RendererID);
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
				//// set the texture wrapping/filtering options (on the currently bound texture object)
				
				if (dataFormat == GL_RED) {
					GLint swizzle[4] = {
						GL_RED,   // Shader Red   channel source = Texture Red
						GL_RED,   // Shader Green channel source = Texture Red
						GL_RED,   // Shader Blue  channel source = Texture Red
						GL_ONE    // Shader Alpha channel source = One
					};
					glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
				}

				if (m_Props.SWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModetoOpenGL(m_Props.SWrapMode));
				if (m_Props.TWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModetoOpenGL(m_Props.TWrapMode));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFiltertoOpenGL(m_Props.MinFilter));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFiltertoOpenGL(m_Props.MagFilter));

				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				Logger::LogError("Failed to load texture : " + path);
				glDeleteTextures(1, &m_RendererID);
				m_Size = { 0, 0 };
				glBindTexture(GL_TEXTURE_2D, 0);
				return;
			}
			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_Size = { width, height };
			m_Handle = glGetTextureHandleARB(m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}

		OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureProps & props)
		{
			m_Props = props;

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, TextureFormattoOpenGL(m_Props.InternalFormat), width, height, 0, TextureFormattoOpenGL(m_Props.DataFormat), TextureDataTypetoOpenGL(m_Props.DataType), NULL);
			if (props.SWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModetoOpenGL(props.SWrapMode));
			if (props.TWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModetoOpenGL(props.TWrapMode));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFiltertoOpenGL(m_Props.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFiltertoOpenGL(m_Props.MagFilter));

			glBindTexture(GL_TEXTURE_2D, 0);
			m_Size = { width, height };

			m_Handle = glGetTextureHandleARB(m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}
		OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, unsigned char * data, const TextureProps & props)
		{
			m_Props = props;

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, TextureFormattoOpenGL(m_Props.InternalFormat), width, height, 0, TextureFormattoOpenGL(m_Props.DataFormat), TextureDataTypetoOpenGL(m_Props.DataType), data);
			if (props.DataFormat == TextureFormat::Monocrome) {
				GLint swizzle[4] = {
					GL_RED,   // Shader Red   channel source = Texture Red
					GL_RED,   // Shader Green channel source = Texture Red
					GL_RED,   // Shader Blue  channel source = Texture Red
					GL_ONE    // Shader Alpha channel source = One
				};
				glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
			}
			if (props.SWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModetoOpenGL(props.SWrapMode));
			if (props.TWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModetoOpenGL(props.TWrapMode));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFiltertoOpenGL(m_Props.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFiltertoOpenGL(m_Props.MagFilter));

			glBindTexture(GL_TEXTURE_2D, 0);
			m_Size = { width, height };

			m_Handle = glGetTextureHandleARB(m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}

		OpenGLTexture2D::~OpenGLTexture2D()
		{
			glMakeImageHandleNonResidentARB(m_Handle);
			glDeleteTextures(1, &m_RendererID);
		}

		void OpenGLTexture2D::Resize(uint32_t width, uint32_t height)
		{
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			if (m_Props.SWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModetoOpenGL(m_Props.SWrapMode));
			if (m_Props.TWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModetoOpenGL(m_Props.TWrapMode));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFiltertoOpenGL(m_Props.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFiltertoOpenGL(m_Props.MagFilter));

			glBindTexture(GL_TEXTURE_2D, 0);
			m_Size = { width, height };

			m_Handle = glGetTextureHandleARB(m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}

		void OpenGLTexture2D::Bind() const
		{
			//glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}
		void OpenGLTexture2D::Unbind() const
		{
			//glBindTexture(GL_TEXTURE_2D, 0);
			glMakeTextureHandleNonResidentARB(m_Handle);
		}


		OpenGLCubeMap::OpenGLCubeMap(const std::vector<TextureProps>& props)
			:m_Props(props)
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
			
			for (uint32_t i = 0; i < 6; i++) {
				int width, height, nrChannels;
				stbi_set_flip_vertically_on_load(m_Props[i].FlipY);
				unsigned char* data = stbi_load(m_Props[i].Path.c_str(), &width, &height, &nrChannels, 0);
				if (data) {

					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0,
						GL_RGB,
						width, height,
						0,
						GL_RGB,
						GL_UNSIGNED_BYTE,
						data
					);
					stbi_image_free(data);
				}
				else {
					Logger::LogWarning("Failed to load texture ( " + m_Props[i].Path + " )");
					stbi_image_free(data);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, TextureFiltertoOpenGL(m_Props[0].MagFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, TextureFiltertoOpenGL(m_Props[0].MinFilter));
			if (m_Props[0].SWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, TextureWrapModetoOpenGL(m_Props[0].SWrapMode));
			if (m_Props[0].TWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, TextureWrapModetoOpenGL(m_Props[0].TWrapMode));
			if (m_Props[0].RWrapMode != TextureWrapMode::None) glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, TextureWrapModetoOpenGL(m_Props[0].RWrapMode));
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			m_Handle = glGetTextureHandleARB(m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}
		OpenGLCubeMap::~OpenGLCubeMap()
		{
		}
		void OpenGLCubeMap::Bind() const
		{
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
			glMakeTextureHandleResidentARB(m_Handle);
		}
		void OpenGLCubeMap::Unbind() const
		{
			//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glMakeTextureHandleNonResidentARB(m_Handle);
		}
}
}