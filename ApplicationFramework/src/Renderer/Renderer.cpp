#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace Engine::Renderer {
	namespace {
		float postProc_Vertex_List[] = {
			-1,-1,0,0,
			1,-1,1,0,
			-1,1,0,1,
			1,1,1,1
		};
		uint32_t postProc_Index_List[] = {
			0,1,2,
			3,2,1
		};

		std::shared_ptr<VertexArray> s_PostProcQuad;

		void initPostProc() {
			std::shared_ptr<VertexBuffer> vbo = VertexBuffer::Create(postProc_Vertex_List, sizeof(postProc_Vertex_List));
			vbo->SetLayout({ {ShaderDataType::Float2, "aPos"},
							{ShaderDataType::Float2, "aTexCoord"} });
			std::shared_ptr<IndexBuffer> ebo = IndexBuffer::Create(postProc_Index_List, sizeof(postProc_Index_List));
		
			s_PostProcQuad = VertexArray::Create();
			s_PostProcQuad->AddVertexBuffer(vbo);
			s_PostProcQuad->SetIndexBuffer(ebo);
		}
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		initPostProc();

		TextureProps PositionBuffer, NormalBuffer, ColorSpecularBuffer;

		ColorSpecularBuffer.InternalFormat = TextureFormat::RGBA;
		ColorSpecularBuffer.DataFormat = TextureFormat::RGBA;
		ColorSpecularBuffer.DataType = TextureDataType::Ubyte;
		ColorSpecularBuffer.MinFilter = TextureFilter::Nearest;
		ColorSpecularBuffer.MagFilter = TextureFilter::Nearest;
		m_Data.ColorSpecBufferIndex = 1;

		m_Data.m_PostProcFrameBuffer = FrameBuffer::Create(true, { ColorSpecularBuffer });
		m_Data.m_GeneralStorageBuffer = ShaderStorageBuffer::Create(0, sizeof(glm::mat4) + sizeof(glm::vec4));
	
		m_Data.m_LightData.m_PointLightStorageBuffer = ShaderStorageBuffer::Create(1, 0);
		m_Data.m_LightData.m_DirectionalLightStorageBuffer = ShaderStorageBuffer::Create(2, 0);
	}
	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		m_Data.m_Width = width;
		m_Data.m_Height = height;
		m_Data.m_PostProcFrameBuffer->Resize(width, height);
		RenderCommand::SetViewport(0, 0, width, height);
		if(m_Data.m_Camera && width != 0 && height != 0) m_Data.m_Camera->onResize(width, height);
	}

	void Renderer::BeginFrame(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Light>>& Lights)
	{
		m_DiagnosticInfo.NumberDrawCalls = 0;
		m_DiagnosticInfo.NumberUniformCalls = 0;
		m_DiagnosticInfo.NumberTriangles = 0;
		m_Data.m_Camera = camera;

		// Store importent information in the main storage buffer.
		uint32_t offset = 0;
		m_Data.m_GeneralStorageBuffer->subData(offset, sizeof(glm::mat4), (const void*)glm::value_ptr(m_Data.m_Camera->getViewProjection())); offset += sizeof(glm::mat4);
		m_Data.m_GeneralStorageBuffer->subData(offset, sizeof(glm::vec4), (const void*)glm::value_ptr(m_Data.m_Camera->getPosition()));		  offset += sizeof(glm::vec4);
		
		std::vector<PointLightData> PLData;
		std::vector<DirectionalLightData> DLData;

		for (auto& l : Lights) {
			switch (l->GetType())
			{
			case LightType::PointLight: {
				std::shared_ptr<PointLight> pl = std::dynamic_pointer_cast<PointLight>(l);
				PLData.push_back(pl->GetData());
				break;
			}
			case LightType::DirectionalLight: {
				std::shared_ptr<DirectionalLight> dl = std::dynamic_pointer_cast<DirectionalLight>(l);
				DLData.push_back(dl->GetData());
				break;
			}
			default:
				break;
			}
		}

		if (PLData.size() != m_Data.m_LightData.LFPointLightCount) {
			m_Data.m_LightData.LFPointLightCount = PLData.size();
			m_Data.m_LightData.m_PointLightStorageBuffer->resize(m_Data.m_LightData.LFPointLightCount * PointLightDataSize);
		}

		m_Data.m_LightData.m_PointLightStorageBuffer->subData(0, m_Data.m_LightData.LFPointLightCount * PointLightDataSize, (const void*)PLData.data());

		if (DLData.size() != m_Data.m_LightData.LFDirectionalLightCount) {
			m_Data.m_LightData.LFDirectionalLightCount = DLData.size();
			m_Data.m_LightData.m_DirectionalLightStorageBuffer->resize(m_Data.m_LightData.LFDirectionalLightCount * DirectionalLightDataSize);
		}
		m_Data.m_LightData.m_DirectionalLightStorageBuffer->subData(0, m_Data.m_LightData.LFDirectionalLightCount * DirectionalLightDataSize, (const void*)DLData.data());
		
		m_Data.m_GeneralStorageBuffer->Bind();
		m_Data.m_LightData.m_PointLightStorageBuffer->Bind();
		m_Data.m_LightData.m_DirectionalLightStorageBuffer->Bind();

		RenderCommand::Clear();
		m_Data.m_PostProcFrameBuffer->Bind();
		RenderCommand::Clear();
	}
	void Renderer::EndFrame()
	{
		m_Data.m_LightData.m_DirectionalLightStorageBuffer->Unbind();
		m_Data.m_LightData.m_PointLightStorageBuffer->Unbind();
		m_Data.m_GeneralStorageBuffer->Unbind();
		m_Data.m_PostProcFrameBuffer->Unbind();
	}
	void Renderer::Render(std::shared_ptr<Shader> postProcShader)
	{
		postProcShader->Bind();
		postProcShader->SetUniform("FrameTexture", UDTexture::Create(m_Data.m_PostProcFrameBuffer->GetTexture(m_Data.ColorSpecBufferIndex)));
		postProcShader->SetUniform("Resolution", UDVec2::Create(m_Data.m_PostProcFrameBuffer->GetSize()));
		RenderCommand::DrawIndexed(s_PostProcQuad);
		postProcShader->Unbind();
	}

	std::shared_ptr<Texture> Renderer::GetRender()
	{
		return m_Data.m_PostProcFrameBuffer->GetTexture(0);
	}

	void Renderer::Submit(std::shared_ptr<Model> model, const glm::mat4& transform, std::shared_ptr<Shader> shader) {
		shader->Bind();
		model->GetMaterialStorageBuffer()->Bind();
		model->GetTextureStorageBuffer()->Bind();

		for (uint32_t i = 0; i < model->GetMeshes().size(); ++i) {
			auto& mesh = model->GetMeshes()[i];
			Submit(mesh, transform, shader);
		}

		model->GetTextureStorageBuffer()->Unbind();
		model->GetMaterialStorageBuffer()->Unbind();
		shader->Unbind();
	}
	void Renderer::Submit(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<Shader> shader) {
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(transform));

		RenderCommand::DrawIndexed(mesh->GetVAO());

		shader->Unbind();
	}
	void Renderer::Submit(std::shared_ptr<VertexArray> VAO, const glm::mat4& transform, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("ViewProjection", UDMat4::Create(m_Data.m_Camera->getViewProjection()));
		shader->SetUniform("Model", UDMat4::Create(transform));
		RenderCommand::DrawIndexed(VAO);

		shader->Unbind();
	}

	void Renderer::ShowRendererDiagnostic()
	{
		/*ImGui::Begin("Renderer Diagnostic");
		ImGui::Text("MS: %f", m_DiagnosticInfo.Milliseconds);
		ImGui::Text("Number Triangles: %d", m_DiagnosticInfo.NumberTriangles);
		ImGui::Text("Number Draw Calls: %d", m_DiagnosticInfo.NumberDrawCalls);
		ImGui::Text("Number Uniform Calls: %d", m_DiagnosticInfo.NumberUniformCalls);
		ImGui::End();*/
	}
}