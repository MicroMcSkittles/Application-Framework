#pragma once
#include "core/Core.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Engine::Renderer {
	enum class TextureWrapMode {
		None,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};
	enum class TextureFormat {
		Monocrome,
		R8,
		RG,
		RG8,
		RGB,
		RGB8,
		RGB16Float,
		RGBA,
		DepthComponent
	};
	enum class TextureDataType {
		Float,
		Ubyte
	};
	enum class TextureFilter {
		Nearest,
		Linear
	};

	struct TextureProps {
		bool FlipY;
		std::string Path;

		// The filter used when shrinking a texture.
		TextureFilter MinFilter = TextureFilter::Linear;
		// The filter used when magnifying a texture.
		TextureFilter MagFilter = TextureFilter::Linear;

		TextureFormat DataFormat = TextureFormat::RGB;
		TextureFormat InternalFormat = TextureFormat::RGB;

		TextureDataType DataType = TextureDataType::Ubyte;

		// Wrap mode for X axis.
		TextureWrapMode SWrapMode = TextureWrapMode::Repeat;
		// Wrap mode for Y axis.
		TextureWrapMode TWrapMode = TextureWrapMode::Repeat;
		// Wrap mode for Z axis.
		TextureWrapMode RWrapMode = TextureWrapMode::Repeat;

		TextureProps(std::string Path = "", bool FlipY = false)
			: Path(Path), FlipY(FlipY) { }
	};
	
	class Texture {
	public:
		virtual ~Texture() { }
	};

	class Texture2D : public Texture {
	public:
		virtual ~Texture2D() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetID() const = 0;
		virtual uint64_t GetHandle() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual TextureProps GetProps() const = 0;

		//virtual void Load() const = 0;
		static std::shared_ptr<Texture2D> Create(const TextureProps& props = TextureProps(), bool CheckHandler = true);
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height, const TextureProps& props = TextureProps());
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height, unsigned char* data, const TextureProps& props = TextureProps());
	};

	class CubeMap : public Texture {
	public:
		virtual ~CubeMap() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetID() const = 0;

		virtual std::vector<TextureProps>& GetProps() = 0;

		static std::shared_ptr<CubeMap> Create(const std::vector<TextureProps>& props, bool CheckHandler = true);
	};
}