#include "Renderer.h"
//#include <imGUI/imgui.h>

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
		m_Data.ColorSpecBufferIndex = 0;

		m_Data.m_PostProcFrameBuffer = FrameBuffer::Create(true, { ColorSpecularBuffer });
	}
	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		m_Data.m_PostProcFrameBuffer->Resize(width, height);
		RenderCommand::SetViewport(0, 0, width, height);
		if(m_Data.m_Camera && width != 0 && height != 0) m_Data.m_Camera->onResize(width, height);
	}

	void Renderer::BeginFrame(std::shared_ptr<Camera> camera)
	{
		m_DiagnosticInfo.NumberDrawCalls = 0;
		m_DiagnosticInfo.NumberUniformCalls = 0;
		m_DiagnosticInfo.NumberTriangles = 0;
		m_Data.m_PostProcFrameBuffer->Bind();
		RenderCommand::Clear();
		m_Data.m_Camera = camera;
	}
	void Renderer::EndFrame()
	{
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
		shader->SetUniform("ViewProjection", UDMat4::Create(m_Data.m_Camera->getViewProjection()));
		shader->SetUniform("CameraPosition", UDVec3::Create(m_Data.m_Camera->getPosition()));

		for (uint32_t i = 0; i < model->GetMeshes().size(); ++i) {
			auto& mesh = model->GetMeshes()[i];
			uint32_t matIndex = model->GetMaterialIndexs()[i];
			Submit(mesh, transform, model->GetMaterials()[matIndex], shader);
		}

		shader->Unbind();
	}
	void Renderer::Submit(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<Shader> shader) {
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(transform));

		RenderCommand::DrawIndexed(mesh->GetVAO());

		shader->Unbind();
	}
	void Renderer::Submit(std::shared_ptr<Mesh> mesh, const glm::mat4 & transform, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniform("Model", UDMat4::Create(transform));

		if (material->GetType() == MaterialType::Textured) {
			std::shared_ptr<TexturedMaterial> mat = std::dynamic_pointer_cast<TexturedMaterial>(material);
			shader->SetUniform("albedoMap", UDTexture::Create(mat->Albedo));
		}
		else if (material->GetType() == MaterialType::Plane) {
			std::shared_ptr<PlaneMaterial> mat = std::dynamic_pointer_cast<PlaneMaterial>(material);
			shader->SetUniform("albedo", UDVec3::Create(mat->Albedo));
			shader->SetUniform("metallic", UDFloat::Create(mat->Metallic));
			shader->SetUniform("roughness", UDFloat::Create(mat->Roughness));
			shader->SetUniform("ao", UDFloat::Create(mat->AO));
		} 

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