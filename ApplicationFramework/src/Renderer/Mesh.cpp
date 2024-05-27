#include "Mesh.h"

namespace Engine::Renderer {
	std::shared_ptr<Mesh> Mesh::Create(const std::vector<float>& vertex_list, const std::vector<uint32_t>& index_list, const BufferLayout& vertex_layout)
	{
		return std::shared_ptr<Mesh>(new Mesh(vertex_list, index_list, vertex_layout));
	}
	std::shared_ptr<Mesh> Mesh::Create(float * vertex_list, uint32_t vertex_list_size, uint32_t * index_list, uint32_t index_list_size, const BufferLayout& vertex_layout)
	{
		return std::shared_ptr<Mesh>(new Mesh(vertex_list, vertex_list_size, index_list, index_list_size, vertex_layout));
	}

	Mesh::Mesh(const std::vector<float>& vertex_list, const std::vector<uint32_t>& index_list, const BufferLayout& vertex_layout)
	{
		m_VertexList = vertex_list;
		m_IndexList = index_list;
		m_VertexLayput = vertex_layout;

		InitMesh();
	}
	Mesh::Mesh(float * vertex_list, uint32_t vertex_list_size, uint32_t * index_list, uint32_t index_list_size, const BufferLayout& vertex_layout)
	{
		m_VertexList = std::vector<float>(vertex_list, vertex_list + (vertex_list_size / sizeof(vertex_list[0])));
		m_IndexList = std::vector<uint32_t>(index_list, index_list + (index_list_size / sizeof(index_list[0])));
		m_VertexLayput = vertex_layout;

		InitMesh();
	}

	void Mesh::InitMesh()
	{
		m_VAO = Renderer::VertexArray::Create();

		std::shared_ptr<Renderer::VertexBuffer> vbo = Renderer::VertexBuffer::Create(&m_VertexList[0], m_VertexList.size() * sizeof(m_VertexList[0]));
		vbo->SetLayout(m_VertexLayput);
		m_VAO->AddVertexBuffer(vbo);

		std::shared_ptr<Renderer::IndexBuffer> ebo = Renderer::IndexBuffer::Create(&m_IndexList[0], m_IndexList.size() * sizeof(m_IndexList[0]));
		m_VAO->SetIndexBuffer(ebo);
	}
}