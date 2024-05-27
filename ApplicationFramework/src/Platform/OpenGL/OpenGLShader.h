#pragma once
#include "Renderer/Shader.h"
#include <map>

class RayTracer;

namespace Engine::Renderer {
	namespace OpenGL {
		class OpenGLUniformList : public UniformList {
		public:
			OpenGLUniformList(int binding, uint32_t elmSize, int count, const void* data);

			virtual void Bind() override;
			virtual void Unbind() override;

			virtual void SetIndex(int index, const void* data) override;
			virtual void SetList(int count, const void* data) override;

		private:
			uint32_t m_RendererID;
			uint32_t m_ElementSize;
			int m_Binding;
		};

		class OpenGLUDLTexture : public UDLTexture {
		public:
			OpenGLUDLTexture(const std::vector<std::shared_ptr<Texture>>& value);
			std::vector<uint64_t> m_TextureHandles;
		};

		class OpenGLShader : public Shader
		{
		public:
			OpenGLShader(std::string vertexPath, std::string fragmentPath);
			OpenGLShader(std::string vertexPath, std::string geometryPath, std::string fragmentPath);

			virtual void Bind() override;
			virtual void Unbind() override;

			virtual void SetUniform(std::string name, std::shared_ptr<UniformData> v) override;
			virtual std::vector<Uniform>& GetUniforms() override { return m_Uniforms; }
		
			uint32_t getid() { return m_RendererID; }

		private:
			void initUniforms();
			std::string findUniforms(std::string& SourceCode);
			std::string preprocSource(std::string SourceCode);

		private:
			std::map<std::string, int> m_UniformLocations;
			std::map<std::string, int> m_UniformIndexs;
			std::vector<Uniform> m_Uniforms;

			uint32_t m_RendererID;
			std::string m_Name;
		};
	}
}