#pragma once
#include "core/Core.h"
#include "Renderer/Buffer.h"
#include "Renderer/Texture.h"
#include "Renderer/UniformData.h"
#include <string>
#include <glm/glm.hpp>

namespace Engine::Renderer {
	class Shader {
	public:
		virtual ~Shader() { }

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetUniform(std::string name, std::shared_ptr<UniformData> v) = 0;
		virtual std::vector<Uniform>& GetUniforms() = 0;

		// Set isSource to true to pass in the shaders source as a string
		static std::shared_ptr<Shader> Create(std::string vertexPath, std::string fragmentPath, bool isSource = false);
		// Set isSource to true to pass in the shaders source as a string
		static std::shared_ptr<Shader> Create(std::string vertexPath, std::string geometryPath, std::string fragmentPath, bool isSource = false);
	};
}