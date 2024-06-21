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

	void Model::PushTexture(std::shared_ptr<Texture2D> texture)
	{
		m_Textures.push_back(texture);
		m_TextureHandles.push_back(texture->GetHandle());
	}

	void Model::CreateSSBs()
	{
		m_MaterialSSB = ShaderStorageBuffer::Create(1, MaterialSize, m_Materials.size());
		m_MaterialSSB->subData(0, m_Materials.size(), (const void*)m_Materials.data());
		
		m_TextureSSB = ShaderStorageBuffer::Create(2, sizeof(uint64_t), m_TextureHandles.size());
		m_TextureSSB->subData(0, m_TextureHandles.size(), (const void*)m_TextureHandles.data());
	}
	void Model::UpdateSSBs()
	{
	}
}