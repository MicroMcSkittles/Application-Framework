#pragma once
#include "core/Core.h"
#include "Mesh.h"
#include "UniformData.h"

namespace Engine::Renderer {
	class Model
	{
	public:
		static std::shared_ptr<Model> Create(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string& directory = "");

		const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
		const std::vector<Material>& GetMaterials() const { return m_Materials; }
		const std::vector<uint64_t>& GetTextureHandles() const { return m_TextureHandles; }
		const std::vector<std::shared_ptr<Texture2D>>& GetTextures() const { return m_Textures; }
		std::shared_ptr<ShaderStorageBuffer> GetMaterialStorageBuffer() const { return m_MaterialSSB; }
		std::shared_ptr<ShaderStorageBuffer> GetTextureStorageBuffer() const { return m_TextureSSB; }
		const std::string& GetDirectory() const { return m_Directory; }

		void PushMesh(std::shared_ptr<Mesh> mesh) { m_Meshes.push_back(mesh); }
		void PushMaterial(Material mat) { m_Materials.push_back(mat); }
		void PushTexture(std::shared_ptr<Texture2D> texture);
		void CreateSSBs();
		void UpdateSSBs();

	private:
		Model(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string& directory);

	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::vector<Material> m_Materials;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		std::vector<uint64_t> m_TextureHandles;
		std::shared_ptr<ShaderStorageBuffer> m_MaterialSSB;
		std::shared_ptr<ShaderStorageBuffer> m_TextureSSB;

		std::string m_Directory;
	};
}