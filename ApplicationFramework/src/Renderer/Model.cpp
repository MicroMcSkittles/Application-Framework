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
}