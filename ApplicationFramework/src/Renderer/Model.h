#pragma once
#include "core/Core.h"
#include "Mesh.h"

namespace Engine::Renderer {
	class Model
	{
	public:
		static std::shared_ptr<Model> Create(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string& directory = "");

		const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
		const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_Materials; }
		const std::vector<uint32_t>& GetMaterialIndexs() const { return m_MaterialIndexs; }
		const std::string& GetDirectory() const { return m_Directory; }

		void PushMesh(std::shared_ptr<Mesh> mesh) { m_Meshes.push_back(mesh); }
		void PushMaterial(std::shared_ptr<Material> mat) { m_Materials.push_back(mat); }
		void PushMaterialIndex(uint32_t index) { m_MaterialIndexs.push_back(index); }

	private:
		Model(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string& directory);

	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::vector<std::shared_ptr<Material>> m_Materials;
		std::vector<uint32_t> m_MaterialIndexs;

		std::string m_Directory;
	};
}