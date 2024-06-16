#include "Model.h"

namespace Engine::Renderer {
	std::shared_ptr<Model> Model::Create(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string & directory)
	{
		return std::shared_ptr<Model>(new Model(meshes, directory));
	}
	Model::Model(const std::vector<std::shared_ptr<Mesh>>& meshes, const std::string & directory)
	{
		m_Meshes = meshes;
		m_Directory = directory;
	}

	void Model::CreateMaterialList()
	{
		m_SSB = ShaderStorageBuffer::Create(1, MaterialSize, m_Materials.size());
		m_SSB->subData(0, m_Materials.size(), (const void*)m_Materials.data());
	}
	void Model::UpdateMaterialList()
	{
	}
}