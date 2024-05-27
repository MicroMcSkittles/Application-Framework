#include "OpenGLShader.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <sstream>
#include <algorithm>

#include "core/Logger.h"
#include "OpenGLTexture.h"
#include "Renderer/Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Engine::Renderer {
	namespace OpenGL {
		std::string loadFromFile(std::string path) {
			using BufIt = std::istreambuf_iterator<char>;
			std::ifstream in(path);
			return std::string(BufIt(in.rdbuf()), BufIt());
		}

		void OpenGLShader::initUniforms()
		{
			for (int index = 0; index < m_Uniforms.size(); ++index) {

				m_UniformLocations.insert(std::pair<std::string, int>(m_Uniforms[index].GetName(), glGetUniformLocation(m_RendererID, m_Uniforms[index].GetName().c_str())));

				switch (m_Uniforms[index].GetType())
				{
				case ShaderDataType::Float: {
					m_Uniforms[index].SetValue(UDFloat::Create(0));
					break;
				}
				case ShaderDataType::Float2: {
					m_Uniforms[index].SetValue(UDVec2::Create(glm::vec2(0, 0)));
					break;
				}
				case ShaderDataType::Float3: {
					m_Uniforms[index].SetValue(UDVec3::Create(glm::vec3(0, 0, 0)));
					break;
				}
				case ShaderDataType::Float4: {
					m_Uniforms[index].SetValue(UDVec4::Create(glm::vec4(0, 0, 0, 0)));
					break;
				}
				case ShaderDataType::Bool: {
					m_Uniforms[index].SetValue(UDBool::Create(false));
					break;
				}
				case ShaderDataType::Int: {
					m_Uniforms[index].SetValue(UDInt::Create(0));
					break;
				}
				case ShaderDataType::Mat2: {
					m_Uniforms[index].SetValue(UDMat2::Create(glm::mat2(1.0f)));
					break;
				}
				case ShaderDataType::Mat3: {
					m_Uniforms[index].SetValue(UDMat3::Create(glm::mat3(1.0f)));
					break;
				}
				case ShaderDataType::Mat4: {
					m_Uniforms[index].SetValue(UDMat4::Create(glm::mat4(1.0f)));
					break;
				}
				case ShaderDataType::Sampler2D: {
					m_Uniforms[index].SetValue(UDTexture::Create(Texture2D::Create(0, 0)));
					break;
				}
				case ShaderDataType::SamplerCube: {
					m_Uniforms[index].SetValue(UDTexture::Create(Texture2D::Create(0, 0)));
					break;
				}
				}
			}
		}

		bool isFlag(std::string str) {
			if (str == "#Exclude") return true;
			return false;
		}

		void removeWord(std::string& SourceCode, std::string word) {
			int found = -1;
			do {
				found = SourceCode.find(word, found + 1);
				if (found != -1) {
					SourceCode = SourceCode.substr(0, found) + SourceCode.substr(found + word.length(), SourceCode.length());
				}
			} while (found != -1);
		}

		uint32_t getFlagsFromWord(std::string word) {
			if (word == "#Exclude") return (uint32_t)UniformFlags::Exclude;
		}

		ShaderDataType getTypeFromString(std::string str) {
			if (str == "float") return ShaderDataType::Float;
			else if (str == "vec2") return ShaderDataType::Float2;
			else if (str == "vec3") return ShaderDataType::Float3;
			else if (str == "vec4") return ShaderDataType::Float4;
			else if (str == "bool") return ShaderDataType::Bool;
			else if (str == "int") return ShaderDataType::Int;
			else if (str == "ivec2") return ShaderDataType::Int2;
			else if (str == "ivec3") return ShaderDataType::Int3;
			else if (str == "ivec4") return ShaderDataType::Int4;
			else if (str == "mat2") return ShaderDataType::Mat2;
			else if (str == "mat3") return ShaderDataType::Mat3;
			else if (str == "mat4") return ShaderDataType::Mat4;
			else if (str == "sampler2D") return ShaderDataType::Sampler2D;
			else if (str == "samplerCube") return ShaderDataType::SamplerCube;
			return ShaderDataType::None;
		}

		std::string OpenGLShader::findUniforms(std::string& SourceCode) {
			std::stringstream ss(SourceCode);
			std::vector<std::string> words;
			std::string word;
			int index = 0;
			bool wordsRemoved = false;
			while (ss >> word)
			{
				if (word == "uniform") {
					int temp = index;
					uint32_t flags = 0;
					word = words[index-=1];
					while (isFlag(word)) {
						flags |= getFlagsFromWord(word);
						word = words[index-=1];
						wordsRemoved = true;
					}
					index = temp;
					std::string type;
					words.push_back(word);
					ss >> word; type = word;
					words.push_back(word);
					ss >> word;
					index+=2;
					if (word[word.size() - 1] == ';') {
						word.pop_back();
						if (word[word.size() - 1] == ']') {
							while (word[word.size() - 1] != '[') {
								word.pop_back();
							}
							word.pop_back();
						}
					}

					m_UniformIndexs.insert(std::pair<std::string, int>(word, m_Uniforms.size()));
					m_Uniforms.push_back({ getTypeFromString(type), word, flags});
				}

				words.push_back(word);
				index++;
			}

			if (!wordsRemoved) return SourceCode;

			removeWord(SourceCode, "#Exclude");

			return SourceCode;
		}

		std::string OpenGLShader::preprocSource(std::string SourceCode)
		{
			for (int i = 0; i < SourceCode.size(); i++) {
				char c = SourceCode[i];
				std::string word;
				while (c != ' ' && c != '\n' && c != '\t') {
					word.push_back(c);
					i++;
					if (i >= SourceCode.size()) break;
					c = SourceCode[i];
				}

				if (word == "#include") {
					int start = i - 8;
					i++;
					if (i >= SourceCode.size())
					{
						Logger::LogError("Path to glsl include required");
						break;
					}
					word = "";
					c = SourceCode[i];
					while (c != ' ' && c != '\n' && c != '\t') {
						word.push_back(c);
						i++;
						if (i >= SourceCode.size()) break;
						c = SourceCode[i];
					}
					std::filesystem::path p(word);
					std::string name = p.filename().string();
					SourceCode = SourceCode.substr(0, start) + preprocSource(loadFromFile(word)) + SourceCode.substr(i, SourceCode.length());
				}
			}

			std::string src = findUniforms(SourceCode);	
			return src;
		}

		OpenGLShader::OpenGLShader(std::string vertexPath, std::string fragmentPath)
		{
			std::filesystem::path p(vertexPath);
			m_Name = p.stem().string();

			std::string vertexSourceCode = loadFromFile(vertexPath);
			std::string fragmentSourceCode =  loadFromFile(fragmentPath);

			vertexSourceCode = preprocSource(vertexSourceCode);
			fragmentSourceCode = preprocSource(fragmentSourceCode);
			//Logger::LogWarning(fragmentSourceCode);

			uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
			uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const char* CvertexSourceCode = vertexSourceCode.c_str();
			const char* CfragmentSourceCode = fragmentSourceCode.c_str();
			glShaderSource(vertexShader, 1, &CvertexSourceCode, 0);
			glShaderSource(fragmentShader, 1, &CfragmentSourceCode, 0);

			glCompileShader(vertexShader);
			glCompileShader(fragmentShader);

			int success;
			char info[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, 0, info);
				Logger::LogError("Failed to compile vertex shader ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, 0, info);
				Logger::LogError("Failed to compile fragment shader ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}

			m_RendererID = glCreateProgram();
			glAttachShader(m_RendererID, vertexShader);
			glAttachShader(m_RendererID, fragmentShader);
			glLinkProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_RendererID, 512, 0, info);
				Logger::LogError("Failed to link shader program ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}

			initUniforms();

			Logger::LogInfo("Successfully loaded shader!");
			return;
		}
		OpenGLShader::OpenGLShader(std::string vertexPath, std::string geometryPath, std::string fragmentPath)
		{
			std::filesystem::path p(vertexPath);
			m_Name = p.stem().string();

			std::string vertexSourceCode = loadFromFile(vertexPath);
			std::string geometrySourceCode = loadFromFile(geometryPath);
			std::string fragmentSourceCode = loadFromFile(fragmentPath);

			vertexSourceCode = preprocSource(vertexSourceCode);
			geometrySourceCode = preprocSource(geometrySourceCode);
			fragmentSourceCode = preprocSource(fragmentSourceCode);
			//Logger::LogWarning(fragmentSourceCode);

			uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
			uint32_t geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const char* CvertexSourceCode = vertexSourceCode.c_str();
			const char* CgeometrySourceCode = geometrySourceCode.c_str();
			const char* CfragmentSourceCode = fragmentSourceCode.c_str();
			glShaderSource(vertexShader, 1, &CvertexSourceCode, 0);
			glShaderSource(geometryShader, 1, &CgeometrySourceCode, 0);
			glShaderSource(fragmentShader, 1, &CfragmentSourceCode, 0);

			glCompileShader(vertexShader);
			glCompileShader(geometryShader);
			glCompileShader(fragmentShader);

			int success;
			char info[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, 0, info);
				Logger::LogError("Failed to compile vertex shader ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometryShader, 512, 0, info);
				Logger::LogError("Failed to compile geometry shader ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, 0, info);
				Logger::LogError("Failed to compile fragment shader ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}

			m_RendererID = glCreateProgram();
			glAttachShader(m_RendererID, vertexShader);
			glAttachShader(m_RendererID, geometryShader);
			glAttachShader(m_RendererID, fragmentShader);
			glLinkProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(geometryShader);
			glDeleteShader(fragmentShader);

			glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_RendererID, 512, 0, info);
				Logger::LogError("Failed to link shader program ( " + m_Name + " ): \n\t" + info);
				m_Uniforms.clear();
				m_UniformIndexs.clear();
				return;
			}

			initUniforms();

			Logger::LogInfo("Successfully loaded shader!");
			return;
		}


		void OpenGLShader::Bind()
		{
			glUseProgram(m_RendererID);
		}
		void OpenGLShader::Unbind()
		{
			glUseProgram(0);
		}

		void OpenGLShader::SetUniform(std::string name, std::shared_ptr<UniformData> v)
		{
			if (!m_UniformIndexs.count(name)) return;
			m_Uniforms[m_UniformIndexs[name]].SetValue(v);
			if(!v->is_list) {
				switch (m_Uniforms[m_UniformIndexs[name]].GetType())
				{
				case ShaderDataType::Float: {
					float value = std::dynamic_pointer_cast<UDFloat>(v)->value;
					glUniform1f(m_UniformLocations[name], value);
					break;
				}
				case ShaderDataType::Float2: {
					glm::vec2 value = std::dynamic_pointer_cast<UDVec2>(v)->value;
					glUniform2f(m_UniformLocations[name], value.x, value.y);
					break;
				}
				case ShaderDataType::Float3: {
					glm::vec3 value = std::dynamic_pointer_cast<UDVec3>(v)->value;
					glUniform3f(m_UniformLocations[name], value.x, value.y, value.z);
					break;
				}
				case ShaderDataType::Float4: {
					glm::vec4 value = std::dynamic_pointer_cast<UDVec4>(v)->value;
					glUniform4f(m_UniformLocations[name], value.x, value.y, value.z, value.w);
					break;
				}
				case ShaderDataType::Bool: {
					bool value = std::dynamic_pointer_cast<UDBool>(v)->value;
					glUniform1i(m_UniformLocations[name], value);
					break;
				}
				case ShaderDataType::Int: {
					int value = std::dynamic_pointer_cast<UDInt>(v)->value;
					glUniform1i(m_UniformLocations[name], value);
					break;
				}
				case ShaderDataType::Mat2: {
					glm::mat2 value = std::dynamic_pointer_cast<UDMat2>(v)->value;
					glUniformMatrix2fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Mat3: {
					glm::mat3 value = std::dynamic_pointer_cast<UDMat3>(v)->value;
					glUniformMatrix3fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Mat4: {
					glm::mat4 value = std::dynamic_pointer_cast<UDMat4>(v)->value;
					glUniformMatrix4fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Sampler2D: {
					std::shared_ptr<OpenGLTexture2D> value = std::dynamic_pointer_cast<OpenGLTexture2D>(std::dynamic_pointer_cast<UDTexture>(v)->value);
					glUniformHandleui64ARB(m_UniformLocations[name], value->GetHandle());
					break;
				}
				case ShaderDataType::SamplerCube: {
					std::shared_ptr<OpenGLCubeMap> value = std::dynamic_pointer_cast<OpenGLCubeMap>(std::dynamic_pointer_cast<UDTexture>(v)->value);
					glUniformHandleui64ARB(m_UniformLocations[name], value->GetHandle());
					break;
				}
				}
			}
			else {
				switch (m_Uniforms[m_UniformIndexs[name]].GetType())
				{
				case ShaderDataType::Float: {
					std::vector<float> value = std::dynamic_pointer_cast<UDLFloat>(v)->value;
					glUniform1fv(m_UniformLocations[name], value.size(), &value[0]);
					break;
				}
				case ShaderDataType::Float2: {
					std::vector<glm::vec2> value = std::dynamic_pointer_cast<UDLVec2>(v)->value;
					glUniform2fv(m_UniformLocations[name], value.size(), glm::value_ptr(value[0]));
					break;
				}
				case ShaderDataType::Float3: {
					std::shared_ptr<UDLVec3> u = std::dynamic_pointer_cast<UDLVec3>(v);
					glUniform3fv(m_UniformLocations[name], u->value.size(), &u->value[0].x);
					break;
				}
				case ShaderDataType::Float4: {
					std::vector<glm::vec4> value = std::dynamic_pointer_cast<UDLVec4>(v)->value;
					glUniform4fv(m_UniformLocations[name], value.size(), glm::value_ptr(value[0]));
					break;
				}
				case ShaderDataType::Bool: {
					std::vector<bool> value = std::dynamic_pointer_cast<UDLBool>(v)->value;
					//glUniform1iv(m_UniformLocations[name], value.size(), &value[0]);
					break;
				}
				case ShaderDataType::Int: {
					std::vector<int> value = std::dynamic_pointer_cast<UDLInt>(v)->value;
					glUniform1iv(m_UniformLocations[name], value.size(), &value[0]);
					break;
				}
				case ShaderDataType::Mat2: {
					glm::mat2 value = std::dynamic_pointer_cast<UDMat2>(v)->value;
					glUniformMatrix2fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Mat3: {
					glm::mat3 value = std::dynamic_pointer_cast<UDMat3>(v)->value;
					glUniformMatrix3fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Mat4: {
					glm::mat4 value = std::dynamic_pointer_cast<UDMat4>(v)->value;
					glUniformMatrix4fv(m_UniformLocations[name], 1, GL_FALSE, &value[0][0]);
					break;
				}
				case ShaderDataType::Sampler2D: {
					std::vector<uint64_t> value = std::dynamic_pointer_cast<OpenGLUDLTexture>(std::dynamic_pointer_cast<UDLTexture>(v))->m_TextureHandles;
					glUniformHandleui64vARB(m_UniformLocations[name], value.size(), &value[0]);
					break;
				}
				}
			}
			Renderer::GetDiagnostic().NumberUniformCalls += 1;
		}
		// UNIFORM LISTS

		OpenGLUniformList::OpenGLUniformList(int binding, uint32_t elmSize, int count, const void* data)
			: m_ElementSize(elmSize), m_Binding(binding)
		{
			glGenBuffers(1, &m_RendererID);
			//glNamedBufferStorage(m_RendererID, m_ElementSize * count, data, GL_DYNAMIC_STORAGE_BIT);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_ElementSize * count, data, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
		}
		void OpenGLUniformList::Bind()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		}
		void OpenGLUniformList::Unbind()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
		void OpenGLUniformList::SetIndex(int index, const void * data)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_ElementSize * index, m_ElementSize, data);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
		void OpenGLUniformList::SetList(int count, const void * data)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_ElementSize * count, data, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		OpenGLUDLTexture::OpenGLUDLTexture(const std::vector<std::shared_ptr<Texture>>& value)
		{
			for (auto &t : value) {
				m_TextureHandles.push_back(std::dynamic_pointer_cast<OpenGLTexture2D>(t)->GetHandle());
			}
		}
}
}