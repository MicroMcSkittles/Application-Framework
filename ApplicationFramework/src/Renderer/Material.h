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

	constexpr uint32_t MaterialSize = sizeof(int) * 5;
	struct Material {
		int AmbientMap_ID;
		int DiffuseMap_ID;
		int SpecularMap_ID;
		int SpecularExponentMap_ID;
		int AlphaMap_ID;
	};
}