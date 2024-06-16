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
		std::shared_ptr<ShaderStorageBuffer> GetStorageBuffer() const { return m_SSB; }
		const std::string& GetDirectory() const { return m_Directory; }

		void PushMesh(std::shared_ptr<Mesh> mesh) { m_Meshes.push_back(mesh); }
		void PushMaterial(Material mat) { m_Materials.push_back(mat); }
		void CreateMaterialList();
		void UpdateMaterialList();

	private:
		Model(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string& directory);

	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::vector<Material> m_Materials;
		std::shared_ptr<ShaderStorageBuffer> m_SSB;

		std::string m_Directory;
	};
}