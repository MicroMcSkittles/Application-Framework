#pragma once
#include "core/Core.h"
#include "Texture.h"
#include <map>

namespace Engine::Renderer {
	class TextureHandler
	{
	public:
		static bool HasTexture(std::string name) { return m_RegTextures.count(name); }
		static void RegTexture(std::shared_ptr<Texture> texture, std::string name) { m_RegTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(name, texture)); }
		static void SetTexture(std::string name, std::shared_ptr<Texture> texture) { m_RegTextures[name] = texture; }
		static std::shared_ptr<Texture> GetTexture(std::string name) { return m_RegTextures[name]; }

	private:
		inline static std::map<std::string, std::shared_ptr<Texture>> m_RegTextures;
	};
}