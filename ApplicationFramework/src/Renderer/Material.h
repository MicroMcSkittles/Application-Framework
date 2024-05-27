#pragma once
#include "core/Core.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <vector>

namespace Engine::Renderer {
	enum MaterialFlags {
		None = BIT(0),
		WireFrame = BIT(1),
		TwoSided = BIT(2),
		DepthTest = BIT(2)
	};
	
	enum class MaterialType {
		None,
		Textured,
		Plane
	};

	struct Material {
		virtual MaterialType GetType() = 0;
		virtual uint32_t GetFlags() = 0;
	};

	struct TexturedMaterial : public Material
	{
		TexturedMaterial() { }
		virtual MaterialType GetType() override {
			return Type;
		}
		virtual uint32_t GetFlags() override {
			return Flags;
		}

		std::shared_ptr<Texture2D> Albedo;
		std::shared_ptr<Texture2D> Normal;
		std::shared_ptr<Texture2D> Roughness;
		std::shared_ptr<Texture2D> Metallic;
		std::shared_ptr<Texture2D> AO;

	private:
		MaterialType Type = MaterialType::Textured;
		uint32_t Flags = MaterialFlags::None;
	};
	struct PlaneMaterial : public Material
	{
		PlaneMaterial() : Albedo(glm::vec3(1.0,1.0,1.0)), 
			Roughness(0.8), Metallic(0.5), AO(0.2) { }

		virtual MaterialType GetType() override {
			return Type;
		}
		virtual uint32_t GetFlags() override {
			return Flags;
		}

		glm::vec3 Albedo;
		float Roughness;
		float Metallic;
		float AO;

	private:
		MaterialType Type = MaterialType::Plane;
		uint32_t Flags = MaterialFlags::None;
	};
}