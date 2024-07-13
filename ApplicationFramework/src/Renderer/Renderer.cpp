#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace Engine::Renderer {
	namespace {
		float Quad_Vertex_List[] = {
			-1,-1,0,0,
			1,-1,1,0,
			-1,1,0,1,
			1,1,1,1
		};
		uint32_t Quad_Index_List[] = {
			0,1,2,
			3,2,1
		};

		std::shared_ptr<VertexArray> s_Quad;

		void initQuad() {
			std::shared_ptr<VertexBuffer> vbo = VertexBuffer::Create(Quad_Vertex_List, sizeof(Quad_Vertex_List));
			vbo->SetLayout({ {ShaderDataType::Float2, "aPos"},
							{ShaderDataType::Float2, "aTexCoord"} });
			std::shared_ptr<IndexBuffer> ebo = IndexBuffer::Create(Quad_Index_List, sizeof(Quad_Index_List));
		
			s_Quad = VertexArray::Create();
			s_Quad->AddVertexBuffer(vbo);
			s_Quad->SetIndexBuffer(ebo);
		}
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		initQuad();

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
	
		Renderer2D::OnWindowResize(width, height);
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
		RenderCommand::DrawIndexed(s_Quad);
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
	
	void Renderer2D::Init()
	{
		TextureProps ColorBuffer;

		ColorBuffer.InternalFormat = TextureFormat::RGBA;
		ColorBuffer.DataFormat = TextureFormat::RGBA;
		ColorBuffer.DataType = TextureDataType::Ubyte;
		ColorBuffer.MinFilter = TextureFilter::Nearest;
		ColorBuffer.MagFilter = TextureFilter::Nearest;

		m_Data.m_PostProcFrameBuffer = FrameBuffer::Create(true, { ColorBuffer });
		m_Data.m_GeneralStorageBuffer = ShaderStorageBuffer::Create(0, sizeof(glm::mat4) + sizeof(glm::vec4));
	}
	void Renderer2D::OnWindowResize(unsigned int width, unsigned int height)
	{
		m_Data.m_Width = width;
		m_Data.m_Height = height;
		m_Data.m_PostProcFrameBuffer->Resize(width, height);
		RenderCommand::SetViewport(0, 0, width, height);
		if (m_Data.m_Camera && width != 0 && height != 0) m_Data.m_Camera->onResize(width, height);
	}
	void Renderer2D::BeginFrame(std::shared_ptr<Camera> camera)
	{
		// Store importent information in the main storage buffer.
		m_Data.m_Camera = camera;
		
		uint32_t offset = 0;
		m_Data.m_GeneralStorageBuffer->subData(offset, sizeof(glm::mat4), (const void*)glm::value_ptr(m_Data.m_Camera->getViewProjection())); offset += sizeof(glm::mat4);
		m_Data.m_GeneralStorageBuffer->subData(offset, sizeof(glm::vec4), (const void*)glm::value_ptr(m_Data.m_Camera->getPosition()));		  offset += sizeof(glm::vec4);
		m_Data.m_GeneralStorageBuffer->Bind();

		RenderCommand::Clear();
		m_Data.m_PostProcFrameBuffer->Bind();
		RenderCommand::Clear();
	}
	void Renderer2D::EndFrame()
	{
		m_Data.m_GeneralStorageBuffer->Unbind();
		m_Data.m_PostProcFrameBuffer->Unbind();
	}
	void Renderer2D::Render(std::shared_ptr<Shader> postProcShader)
	{
		postProcShader->Bind();
		postProcShader->SetUniform("FrameTexture", UDTexture::Create(m_Data.m_PostProcFrameBuffer->GetTexture(1)));
		postProcShader->SetUniform("Resolution", UDVec2::Create(m_Data.m_PostProcFrameBuffer->GetSize()));
		RenderCommand::DrawIndexed(s_Quad);
		postProcShader->Unbind();
	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 tint, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(glm::translate(glm::scale(glm::mat4(1), {scale, 1}), {position, 0})));
		shader->SetUniform("Color", UDVec3::Create(color));
		shader->SetUniform("Tint", UDVec3::Create(tint));

		RenderCommand::DrawIndexed(s_Quad);

		shader->Unbind();
	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, std::shared_ptr<Texture2D> texture, glm::vec3 tint, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(glm::translate(glm::scale(glm::mat4(1), { scale, 1 }), { position, 0 })));
		shader->SetUniform("ColorTexture", UDTexture::Create(texture));
		shader->SetUniform("Tint", UDVec3::Create(tint));
		
		RenderCommand::DrawIndexed(s_Quad);

		shader->Unbind();
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec3 color, glm::vec3 tint, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(glm::rotate(glm::translate(glm::scale(glm::mat4(1), { scale, 1 }), {position, 0}), rotation, { 0,0,1 })));
		shader->SetUniform("Color", UDVec3::Create(color));
		shader->SetUniform("Tint", UDVec3::Create(tint));

		RenderCommand::DrawIndexed(s_Quad);

		shader->Unbind();
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 scale, float rotation, std::shared_ptr<Texture2D> texture, glm::vec3 tint, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(glm::rotate(glm::translate(glm::scale(glm::mat4(1), { scale, 1 }), { position, 0 }), rotation, { 0,0,1 })));
		shader->SetUniform("ColorTexture", UDTexture::Create(texture));
		shader->SetUniform("Tint", UDVec3::Create(tint));

		RenderCommand::DrawIndexed(s_Quad);

		shader->Unbind();
	}
	void Renderer2D::ShowRendererDiagnostic()
	{
		Renderer::ShowRendererDiagnostic();
	}
}