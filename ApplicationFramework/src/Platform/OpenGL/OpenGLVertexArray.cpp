#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Engine::Renderer {
	namespace OpenGL {

		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
			switch (type)
			{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
			}

			std::cout << "Unknown ShaderDataType\n";
			return 0;
		}

		OpenGLVertexArray::OpenGLVertexArray()
		{
			glGenVertexArrays(1, &m_RenderID);
			glBindVertexArray(m_RenderID);
		}

		void OpenGLVertexArray::Bind() const
		{
			glBindVertexArray(m_RenderID);
		}
		void OpenGLVertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}

		void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
		{
			if (!vertexBuffer->GetLayout().GetElements().size()) {
				std::cout << "No buffer layout\n";
				return;
			}

			glBindVertexArray(m_RenderID);
			vertexBuffer->Bind();

			uint32_t index = 0;
			const auto& layout = vertexBuffer->GetLayout();
			for (const auto& element : layout) {
				glVertexAttribPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(void*)element.Offset);
				glEnableVertexAttribArray(index);
				++index;
			}
			m_VertexBuffers.push_back(vertexBuffer);
		}
		void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			glBindVertexArray(m_RenderID);
			indexBuffer->Bind();
			m_IndexBuffer = indexBuffer;
		}

		const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
		{
			return m_VertexBuffers;
		}
		const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
		{
			return m_IndexBuffer;
		}
	}
}