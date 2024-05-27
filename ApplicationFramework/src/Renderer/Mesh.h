#pragma once
#include "core/Core.h"
#include "VertexArray.h"
#include "Material.h"

namespace Engine::Renderer {
	class Mesh
	{
	public:
		static std::shared_ptr<Mesh> Create(const std::vector<float>& vertex_list, const std::vector<uint32_t>& index_list, 
			const BufferLayout& vertex_layout = { {ShaderDataType::Float3, "aPos"} });
		static std::shared_ptr<Mesh> Create(float* vertex_list, uint32_t vertex_list_size, uint32_t* index_list, uint32_t index_list_size,
			const BufferLayout& vertex_layout = { {ShaderDataType::Float3, "aPos"} });

		std::shared_ptr<VertexArray> GetVAO() const { return m_VAO; }
		const std::vector<float>& GetVertexList() const { return m_VertexList; }
		const std::vector<uint32_t>& GetIndexList() const { return m_IndexList; }

	private:
		Mesh(const std::vector<float>& vertex_list, const std::vector<uint32_t>& index_list, const BufferLayout& vertex_layout);
		Mesh(float* vertex_list, uint32_t vertex_list_size, uint32_t* index_list, uint32_t index_list_size, const BufferLayout& vertex_layout);

		void InitMesh();

	private:
		std::vector<float> m_VertexList;
		std::vector<uint32_t> m_IndexList;

		std::shared_ptr<VertexArray> m_VAO;
		BufferLayout m_VertexLayput;
	};
}