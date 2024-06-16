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

	constexpr uint32_t MaterialSize = sizeof(float) * 4;
	struct Material {
		glm::vec3 Albedo = { 0.0f, 0.0f, 0.0f };
		float PAD1 = 0.0f;
	};
}