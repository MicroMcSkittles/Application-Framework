#include "OBJLoader.h"
#include <filesystem>
#include <tiny_obj_loader.h>
#include "core/Logger.h"

namespace Engine::AssetLoader {

    Renderer::BufferLayout VertexLayout = {
            {Renderer::ShaderDataType::Float3, "aPosition"},
            {Renderer::ShaderDataType::Float3, "aNormal"},
            {Renderer::ShaderDataType::Float2, "aTexCoord"},
            {Renderer::ShaderDataType::Float,  "aMatID" }
    };

    std::shared_ptr<Renderer::Model> OBJLoader::LoadModel(const std::string& path)
    {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> tol_materials;
        std::string warning, error;

        
        if (!tinyobj::LoadObj(&attributes, &shapes, &tol_materials, &warning, &error, path.c_str(), "assets/Models/")) {
            Logger::LogWarning("Failed to load obj model at path ( " + path + " )\nTiny Obj Loader warning ( " + warning + " )\nTiny Obj Loader error ( " + error + " )");
        }

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

        std::shared_ptr<Renderer::Model> model = Renderer::Model::Create(meshes, path);

        for (const auto& m : tol_materials) {
            Renderer::Material Material;

            // Ambient value
            {
                if (m.ambient_texname == "") {
                    unsigned char data[3] = {
                        m.ambient[0] * 255,
                        m.ambient[1] * 255,
                        m.ambient[2] * 255,
                    };
                    model->PushTexture(Renderer::Texture2D::Create(1, 1, data));
                }
                else {
                    //TODO: Edit texture props
                    std::filesystem::path p = path;

                    Renderer::TextureProps props;
                    props.Path = p.parent_path().string() + "/" + m.ambient_texname;
                    model->PushTexture(Renderer::Texture2D::Create(props));
                }
                Material.AmbientMap_ID = model->GetTextureHandles().size() - 1;
            }

            // Diffuse value
            {
                if (m.diffuse_texname == "") {
                    unsigned char data[3] = {
                        m.diffuse[0] * 255,
                        m.diffuse[1] * 255,
                        m.diffuse[2] * 255,
                    };
                    model->PushTexture(Renderer::Texture2D::Create(1, 1, data));
                }
                else {
                    //TODO: Edit texture props
                    std::filesystem::path p = path;

                    Renderer::TextureProps props;
                    props.Path = p.parent_path().string() + "/" + m.diffuse_texname;
                    model->PushTexture(Renderer::Texture2D::Create(props));
                }
                Material.DiffuseMap_ID = model->GetTextureHandles().size() - 1;
            }

            // Specular value
            {
                if (m.specular_texname == "") {
                    unsigned char data[3] = {
                        m.specular[0] * 255,
                        m.specular[1] * 255,
                        m.specular[2] * 255,
                    };
                    model->PushTexture(Renderer::Texture2D::Create(1, 1, data));
                }
                else {
                    //TODO: Edit texture props
                    std::filesystem::path p = path;

                    Renderer::TextureProps props;
                    props.Path = p.parent_path().string() + "/" + m.specular_texname;
                    model->PushTexture(Renderer::Texture2D::Create(props));
                }
                Material.SpecularMap_ID = model->GetTextureHandles().size() - 1;
            }

            // Specular Exponent value
            {
                if (m.specular_highlight_texname == "") {
                    unsigned char data[1] = {
                        255
                    };
                    Renderer::TextureProps props;
                    props.DataFormat = Renderer::TextureFormat::Monocrome;
                    props.InternalFormat = Renderer::TextureFormat::R8;
                    model->PushTexture(Renderer::Texture2D::Create(1, 1, data));
                }
                else {
                    //TODO: Edit texture props
                    std::filesystem::path p = path;

                    Renderer::TextureProps props;
                    props.Path = p.parent_path().string() + "/" + m.specular_highlight_texname;
                    model->PushTexture(Renderer::Texture2D::Create(props));
                }
                Material.SpecularExponentMap_ID = model->GetTextureHandles().size() - 1;
            }
            
            // Alpha value
            {
                if (m.alpha_texname == "") {
                    unsigned char data[1] = {
                        255
                    };
                    Renderer::TextureProps props;
                    props.DataFormat = Renderer::TextureFormat::Monocrome;
                    props.InternalFormat = Renderer::TextureFormat::R8;
                    model->PushTexture(Renderer::Texture2D::Create(1, 1, data));
                }
                else {
                    //TODO: Edit texture props
                    std::filesystem::path p = path;

                    Renderer::TextureProps props;
                    props.Path = p.parent_path().string() + "/" + m.alpha_texname;
                    model->PushTexture(Renderer::Texture2D::Create(props));
                }
                Material.AlphaMap_ID = model->GetTextureHandles().size() - 1;
            }

            model->PushMaterial(Material);
        }

        model->CreateSSBs();

        return model;
    }
    std::shared_ptr<Renderer::Mesh> OBJLoader::LoadMesh(const std::string & path)
    {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> tol_materials;
        std::string warning, error;


        if (!tinyobj::LoadObj(&attributes, &shapes, &tol_materials, &warning, &error, path.c_str(), "assets/Models/")) {
            Logger::LogWarning("Failed to load obj model at path ( " + path + " )\nTiny Obj Loader warning ( " + warning + " )\nTiny Obj Loader error ( " + error + " )");
        }

        std::vector<std::shared_ptr<Renderer::Mesh>> meshes;

        tinyobj::shape_t shape = shapes[0];

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

        std::shared_ptr<Renderer::Mesh> mesh = Renderer::Mesh::Create(vertices, indices, VertexLayout);

        

        return mesh;
    }
}