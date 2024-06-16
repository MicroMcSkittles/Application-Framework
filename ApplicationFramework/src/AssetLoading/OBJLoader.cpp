#include "OBJLoader.h"
#include <tiny_obj_loader.h>
#include "core/Logger.h"

namespace Engine::AssetLoader {
    std::shared_ptr<Renderer::Model> OBJLoader::Load(const std::string& path)
    {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> tol_materials;
        std::string warning, error;

        
        if (!tinyobj::LoadObj(&attributes, &shapes, &tol_materials, &warning, &error, path.c_str(), "assets/Models/")) {
            Logger::LogWarning("Failed to load obj model at path ( " + path + " )\nTiny Obj Loader warning ( " + warning + " )\nTiny Obj Loader error ( " + error + " )");
        }

        Renderer::BufferLayout VertexLayout = {
            {Renderer::ShaderDataType::Float3, "aPosition"},
            {Renderer::ShaderDataType::Float3, "aNormal"},
            {Renderer::ShaderDataType::Float2, "aTexCoord"},
            {Renderer::ShaderDataType::Float,  "aMatID" }
        };
        std::vector<std::shared_ptr<Renderer::Mesh>> meshes;

        for (uint32_t i = 0; i < shapes.size(); ++i) {
            tinyobj::shape_t shape = shapes[i];

            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            uint32_t index_offset = 0;

            for (uint32_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
                uint32_t fv = uint32_t(shape.mesh.num_face_vertices[f]);

                for (uint32_t v = 0; v < fv; ++v) {
                    tinyobj::index_t index = shape.mesh.indices[v + index_offset];

                    indices.push_back(v + index_offset);

                    // Vertices
                    vertices.push_back(attributes.vertices[index.vertex_index * 3 + 0]);
                    vertices.push_back(attributes.vertices[index.vertex_index * 3 + 1]);
                    vertices.push_back(attributes.vertices[index.vertex_index * 3 + 2]);

                    // Normals
                    vertices.push_back(attributes.normals[index.normal_index * 3 + 0]);
                    vertices.push_back(attributes.normals[index.normal_index * 3 + 1]);
                    vertices.push_back(attributes.normals[index.normal_index * 3 + 2]);

                    // Texcoords
                    vertices.push_back(attributes.texcoords[index.texcoord_index * 2 + 0]);
                    vertices.push_back(attributes.texcoords[index.texcoord_index * 2 + 1]);

                    // Material ID
                    vertices.push_back((float)shape.mesh.material_ids[f]);
                }
                index_offset += fv;
            }
            meshes.push_back(Renderer::Mesh::Create(vertices, indices, VertexLayout));
        }

        std::shared_ptr<Renderer::Model> model = Renderer::Model::Create(meshes);

        for (const auto& m : tol_materials) {
            Renderer::Material Material;

            glm::vec3 albedo = { 
                m.diffuse[0],
                m.diffuse[1],
                m.diffuse[2]
            };
            Material.Albedo = albedo;

            model->PushMaterial(Material);
        }

        model->CreateMaterialList();

        return model;
    }
}