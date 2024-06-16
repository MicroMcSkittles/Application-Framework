#pragma once
#include <glm/glm.hpp>
#include "Renderer/Model.h"

namespace Engine::AssetLoader {
	class OBJLoader
	{
	public:
		// Model has a vertex layout of Float3 aPos, Float3 aNormal, Float2 aTexCoord, Float aMatID
		static std::shared_ptr<Renderer::Model> Load(const std::string& path);
	};
}