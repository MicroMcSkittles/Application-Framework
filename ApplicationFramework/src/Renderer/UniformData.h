#pragma once
#include "core/Core.h"
#include "Renderer/Buffer.h"
#include "Renderer/Texture.h"
#include <string>
#include <glm/glm.hpp>

namespace Engine::Renderer {
	enum class UniformFlags {
		None = BIT(0),
		Exclude = BIT(1)
	};

	class UniformData {
	public:
		UniformData(bool is_list = false): is_list(is_list) {}
		virtual ~UniformData() {}

		bool is_list;
	};
	class UDTexture : public UniformData {
	public:
		static std::shared_ptr<UDTexture> Create(std::shared_ptr<Texture> value) {
			return std::shared_ptr<UDTexture>(new UDTexture(value));
		}

		std::shared_ptr<Texture> value;
	private:
		UDTexture(std::shared_ptr<Texture> value) : value(value) { }
	};
	class UDFloat : public UniformData {
	public:
		static std::shared_ptr<UDFloat> Create(float value) {
			return std::shared_ptr<UDFloat>(new UDFloat(value));
		}

		float value;
	private:
		UDFloat(float value) : value(value) { }
	};
	class UDVec2 : public UniformData {
	public:
		static std::shared_ptr<UDVec2> Create(glm::vec2 value) {
			return std::shared_ptr<UDVec2>(new UDVec2(value));
		}

		glm::vec2 value;
	private:
		UDVec2(glm::vec2 value) : value(value) { }
	};
	class UDVec3 : public UniformData {
	public:
		static std::shared_ptr<UDVec3> Create(glm::vec3 value) {
			return std::shared_ptr<UDVec3>(new UDVec3(value));
		}

		glm::vec3 value;
	private:
		UDVec3(glm::vec3 value) : value(value) { }
	};
	class UDVec4 : public UniformData {
	public:
		static std::shared_ptr<UDVec4> Create(glm::vec4 value) {
			return std::shared_ptr<UDVec4>(new UDVec4(value));
		}

		glm::vec4 value;
	private:
		UDVec4(glm::vec4 value) : value(value) { }
	};
	class UDBool : public UniformData {
	public:
		static std::shared_ptr<UDBool> Create(bool value) {
			return std::shared_ptr<UDBool>(new UDBool(value));
		}

		bool value;
	private:
		UDBool(bool value) : value(value) { }
	};
	class UDInt : public UniformData {
	public:
		static std::shared_ptr<UDInt> Create(int value) {
			return std::shared_ptr<UDInt>(new UDInt(value));
		}

		int value;
	private:
		UDInt(int value) : value(value) { }
	};
	class UDMat2 : public UniformData {
	public:
		static std::shared_ptr<UDMat2> Create(glm::mat2 value) {
			return std::shared_ptr<UDMat2>(new UDMat2(value));
		}

		glm::mat2 value;
	private:
		UDMat2(glm::mat2 value) : value(value) { }
	};
	class UDMat3 : public UniformData {
	public:
		static std::shared_ptr<UDMat3> Create(glm::mat3 value) {
			return std::shared_ptr<UDMat3>(new UDMat3(value));
		}

		glm::mat3 value;
	private:
		UDMat3(glm::mat3 value) : value(value) { }
	};
	class UDMat4 : public UniformData {
	public:
		static std::shared_ptr<UDMat4> Create(glm::mat4 value) {
			return std::shared_ptr<UDMat4>(new UDMat4(value));
		}

		glm::mat4 value;
	private:
		UDMat4(glm::mat4 value) : value(value) { }
	};

	class UniformDataList : public UniformData {
	public:
		UniformDataList() : UniformData(true) {}
		virtual ~UniformDataList() {}
	};
	class UDLTexture : public UniformDataList {
	public:
		static std::shared_ptr<UDLTexture> Create(const std::vector<std::shared_ptr<Texture>>& value);
	};
	class UDLFloat : public UniformDataList {
	public:
		static std::shared_ptr<UDLFloat> Create(std::vector<float> value) {
			return std::shared_ptr<UDLFloat>(new UDLFloat(value));
		}

		std::vector<float> value;
	private:
		UDLFloat(std::vector<float> value) : value(value) { is_list = true; }
	};
	class UDLVec2 : public UniformDataList {
	public:
		static std::shared_ptr<UDLVec2> Create(std::vector<glm::vec2> value) {
			return std::shared_ptr<UDLVec2>(new UDLVec2(value));
		}

		std::vector<glm::vec2> value;
	private:
		UDLVec2(std::vector<glm::vec2> value) : value(value) { }
	};
	class UDLVec3 : public UniformDataList {
	public:
		static std::shared_ptr<UDLVec3> Create(std::vector<glm::vec3> value) {
			return std::shared_ptr<UDLVec3>(new UDLVec3(value));
		}

		std::vector<glm::vec3> value;
	private:
		UDLVec3(std::vector<glm::vec3> value) : value(value) { }
	};
	class UDLVec4 : public UniformDataList {
	public:
		static std::shared_ptr<UDLVec4> Create(std::vector<glm::vec4> value) {
			return std::shared_ptr<UDLVec4>(new UDLVec4(value));
		}

		std::vector<glm::vec4> value;
	private:
		UDLVec4(std::vector<glm::vec4> value) : value(value) { }
	};
	class UDLBool : public UniformDataList {
	public:
		static std::shared_ptr<UDLBool> Create(std::vector<bool> value) {
			return std::shared_ptr<UDLBool>(new UDLBool(value));
		}

		std::vector<bool> value;
	private:
		UDLBool(std::vector<bool> value) : value(value) { }
	};
	class UDLInt : public UniformDataList {
	public:
		static std::shared_ptr<UDLInt> Create(std::vector<int> value) {
			return std::shared_ptr<UDLInt>(new UDLInt(value));
		}

		std::vector<int> value;
	private:
		UDLInt(std::vector<int> value) : value(value) { }
	};
	class UDLMat2 : public UniformDataList {
	public:
		static std::shared_ptr<UDLMat2> Create(std::vector<glm::mat2> value) {
			return std::shared_ptr<UDLMat2>(new UDLMat2(value));
		}

		std::vector<glm::mat2> value;
	private:
		UDLMat2(std::vector<glm::mat2> value) : value(value) { }
	};
	class UDLMat3 : public UniformDataList {
	public:
		static std::shared_ptr<UDLMat3> Create(std::vector<glm::mat3> value) {
			return std::shared_ptr<UDLMat3>(new UDLMat3(value));
		}

		std::vector<glm::mat3> value;
	private:
		UDLMat3(std::vector<glm::mat3> value) : value(value) { }
	};
	class UDLMat4 : public UniformDataList {
	public:
		static std::shared_ptr<UDLMat4> Create(std::vector<glm::mat4> value) {
			return std::shared_ptr<UDLMat4>(new UDLMat4(value));
		}

		std::vector<glm::mat4> value;
	private:
		UDLMat4(std::vector<glm::mat4> value) : value(value) { }
	};

	class Uniform {
	public:
		Uniform(ShaderDataType type, std::string name, uint32_t flags)
			: m_Type(type), m_Name(name), m_Flags(flags) {
			m_Value = std::shared_ptr<UniformData>(new UniformData());
			m_IsInit = false;
		}

		bool IsInit() { return m_IsInit; }
		ShaderDataType GetType() { return m_Type; }
		uint32_t GetFlags() { return m_Flags; }
		std::string GetName() { return m_Name; }
		std::shared_ptr<UniformData> GetValue() { return m_Value; }
		void SetValue(std::shared_ptr<UniformData> v) { m_Value = v; m_IsInit = true; }

	private:
		bool m_IsInit;
		ShaderDataType m_Type;
		std::string m_Name;
		std::shared_ptr<UniformData> m_Value;
		uint32_t m_Flags;
	};

	class UniformList {
	public:
		virtual ~UniformList() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetIndex(int index, const void* data) = 0;
		virtual void SetList(int count, const void* data) = 0;

		// elmSize is the size of each element in the list.
		static std::shared_ptr<UniformList> Create(int binding, uint32_t elmSize, int count, const void* data);
	};
}