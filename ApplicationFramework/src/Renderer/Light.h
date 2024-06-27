#pragma once
#include <glm/glm.hpp>

namespace Engine::Renderer {
	enum class LightType {
		None = 0,
		PointLight,
		DirectionalLight,
		SpotLight,
	};

	class Light {
	public:
		virtual ~Light() { }
		virtual LightType GetType() const = 0;
	};

	struct PointLightData
	{
		glm::vec3 Position;
		float _PAD_1;
		glm::vec3 Color;
		float _PAD_2;
		float Constant;
		float Linear;
		float Quadratic;
		float _PAD_3;
	};
	const uint32_t PointLightDataSize = sizeof(PointLightData);

	class PointLight : public Light {
	public:
		static std::shared_ptr<PointLight> Create(glm::vec3 Position, glm::vec3 Color = glm::vec3(1.0f),
			float Constant = 1.0f, float Linear = 0.09, float Quadratic = 0.032f) {
			return std::shared_ptr<PointLight>(new PointLight(Position, Color, Constant, Linear, Quadratic));
		}

		virtual LightType GetType() const override {
			return LightType::PointLight;
		}

		const PointLightData& GetData() const {
			return m_Data;
		}

	private:
		PointLight(glm::vec3 Position, glm::vec3 Color = glm::vec3(1.0f),
			float Constant = 1.0f, float Linear = 0.09, float Quadratic = 0.032f) {
			m_Data.Position = Position;
			m_Data.Color = Color;
			m_Data.Constant = Constant;
			m_Data.Linear = Linear;
			m_Data.Quadratic = Quadratic;
		}
	private:
		PointLightData m_Data;
	};

	struct DirectionalLightData
	{
		glm::vec3 Direction;
		float _PAD_1;
		glm::vec3 Color;
		float _PAD_2;
	};
	const uint32_t DirectionalLightDataSize = sizeof(DirectionalLightData);

	class DirectionalLight : public Light {
	public:
		static std::shared_ptr<DirectionalLight> Create(glm::vec3 Direction, glm::vec3 Color = { 1.0f, 1.0f, 1.0f }) {
			return std::shared_ptr<DirectionalLight>(new DirectionalLight(Direction, Color));
		}

		virtual LightType GetType() const override {
			return LightType::DirectionalLight;
		}

		const DirectionalLightData& GetData() const {
			return m_Data;
		}

	private:
		DirectionalLight(glm::vec3 Direction, glm::vec3 Color) {
			m_Data.Direction = Direction;
			m_Data.Color = Color;
		}
	private:
		DirectionalLightData m_Data;
	};
}