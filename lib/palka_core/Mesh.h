//
// Created by dimka on 11/30/2021.
//

#ifndef PALKA_MESH_H
#define PALKA_MESH_H

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "ConsoleLog.h"
#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <tiny_gltf.h>

namespace palka
{
    class Material
    {
    public:
        enum class Type : int
        {
            TEXTURE_ALBEDO = 0,
            TEXTURE_ROUGHNESS,
            TEXTURE_NORMAL,
            TEXTURE_METALLIC,
            TEXTURE_EMISSIVE,
            TEXTURE_OCCLUSION
        };
        static constexpr std::string_view type_table[] = {"texture_albedo", "texture_roughness", "texture_normal", "texture_metallic", "texture_emissive",
                                                          "texture_occlusion"};
        using Ptr = Material*;
        Texture texture;
        Type type;

        Material(Texture textures, Type type) : texture(std::move(textures)), type(type)
        {

        }
    };

    struct _Material
    {
        std::string name;
        std::map<Material::Type, std::string> textureMap;

        _Material(std::string name) : name(std::move(name))
        {
        }
    };

    class Mesh
    {
        RawBufferStorage raw_storage;
        VAOBindMap vao_map;
        std::vector<Material::Ptr> materials;
        unsigned int index_count;
        bool useMaterials;
    public:

        Mesh(RawBufferStorage raw_storage, VAOBindMap vao_map, unsigned int index_count) :
                raw_storage(std::move(raw_storage)), index_count(index_count), vao_map(std::move(vao_map)), useMaterials(false)
        {

        }

        Mesh(RawBufferStorage raw_storage, VAOBindMap vao_map, unsigned int index_count, std::vector<Material::Ptr> materials) :
                raw_storage(std::move(raw_storage)), index_count(index_count), vao_map(std::move(vao_map)), materials(std::move(materials)), useMaterials(true)
        {

        }

        void makeVBO(VertexArrayObject& vao)
        {
            vao.bind();
            //VertexBufferObject indecesVbo(BufferObject::BufferType::EAB);
            //  VertexBufferObject vbo;
            //index_count = indecies.size();

            // vbo.create(sizeof(Vec3f) * array.size());
            //indecesVbo.create(sizeof(GLuint) * indecies.size());

            // vbo.setData(sizeof(Vec3f) * array.size(), &array[0].x);
            //indecesVbo.setData(sizeof(GLuint) * indecies.size(), &indecies[0]);
            //vao.setPointers(vbo, sizeof(Vec3f));
            // vao.setPointer_u(vbo, 0, 3, GL_FLOAT, sizeof(Vec3f), 0);

            std::map<int, VertexBufferObject> vbos;
            Console::fmt_log_skip_uniq("create vbo from vbo_map and bind to vao", Console::info);
            for(auto &[id, buffer]: raw_storage)
            {
                VertexBufferObject vbo(buffer.type);
                vbo.create(buffer.bytesAdded);
                vbo.setData(buffer.bytesAdded, buffer.data.data());
                vbos.emplace(std::make_pair(id, std::move(vbo)));
            }
            for(auto &[point, vao_]: vao_map)
                vao.setPointer_u(vbos[vao_.vbo_id], point, vao_.size_type,
                                 vao_.type, vao_.stride, vao_.offset, vao_.normilized);
            vao.unbind();

        }
        void render(ShaderProgram& shader)
        {
            int i = 0;
            for(auto mat: materials)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                Material::Type name = mat->type;
//                if(name == Material::Type::TEXTURE_ALBEDO)
//                    number = std::to_string(albedo++);
//                else if(name == Material::Type::TEXTURE_ROUGHNESS)
//                    number = std::to_string(rough++);
//                else if(name == Material::Type::TEXTURE_NORMAL)
//                    number = std::to_string(normal++);
//                else if(name == Material::Type::TEXTURE_METALLIC)
//                    number = std::to_string(metallic++);

                glUniform1i(shader.getUniformLoc(Material::type_table[(int) mat->type]), i);
                glBindTexture(GL_TEXTURE_2D, mat->texture.textureID);
                ++i;
            }
            glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
        }
    };

    class Model
    {
    public:
        std::vector<Mesh> meshes;
        bool isInit;
        //int index_count;
        VertexArrayObject vao;
        std::multimap<int, Material> materials;

        Model() : isInit(false)
        {
        }

        void setMaterials(std::multimap<int, Material>&& mats)
        {
            materials = std::move(mats);
        }

        void init()
        {
            if(!isInit)
            {
                vao.create(1);
                for(auto& m: meshes)
                    m.makeVBO(vao);
            } else Console::addLog("Attempt to initialize a model that has already been initialized", Console::error);
        }

        void render(ShaderProgram& shader)
        {
            vao.bind();
            for(auto& m: meshes)
                m.render(shader);
        }
    };

    class MaterialMap
    {
    public:
        std::map<int, _Material> materials;
    };

    class gltf_loader
    {
        std::vector<Mesh> meshes;
        MaterialMap mat_map;
        std::multimap<int, Material> txMap;
        // RawBufferStorage raw_buffer;
        // VAOBindMap vao_map;

    public:
        tinygltf::TinyGLTF gltf_ctx;

        std::string GetFilePathExtension(const std::string& FileName)
        {
            if(FileName.find_last_of(".") != std::string::npos)
                return FileName.substr(FileName.find_last_of(".") + 1);
            return "";
        }

        template<class T>
        auto buff_offset(T i)
        {
            return (char*) NULL + i;
        }

        tinygltf::Model load(const std::string& path)
        {
            std::string err;
            std::string warn;
            auto ext = GetFilePathExtension(path);
            tinygltf::Model model;
            if(ext.compare("glb") == 0)
            {
                gltf_ctx.LoadBinaryFromFile(&model, &err, &warn,
                                            path.data());
            } else
            {
                gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, path.data());
            }
            return model;
        }

        Mesh bindMesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
        {
            RawBufferStorage raw_buffer;
            VAOBindMap vao_map;
            // std::map<int, VertexBufferObject> vbos;
            int i = 0;
            //vao.bind();
            for(auto& bufferView: model.bufferViews)
            {
                if(bufferView.target == 0)
                {
                    Console::addLog("bufferView is zero", Console::error);
                    continue;
                }
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                Console::fmt_log_skip_uniq("bufferview.target: {}", Console::info, bufferView.target);
                BufferObject::BufferType t;
                if(bufferView.target == 34962)
                    t = BufferObject::VBO;
                else if(bufferView.target == 34963)
                    t = BufferObject::BufferType::EAB;
                else
                {
                    assert(!"buffer?");
                }

                raw_buffer.setData(i, {.type = t, .data = std::vector<unsigned char>(std::begin(buffer.data) + bufferView.byteOffset,
                                                                                     std::begin(buffer.data) + bufferView.byteOffset +
                                                                                     bufferView.byteLength),
                        .bytesAdded = bufferView.byteLength});
                Console::fmt_log_skip_uniq("add raw buffer id: {} request size: {} actual size: {}", Console::info,
                                           i, bufferView.byteLength, raw_buffer[i].data.size());
                i++;
            }
            unsigned int index_sz;
            std::vector<Material::Ptr> mat_ptr;
            for(size_t i = 0; i < mesh.primitives.size(); ++i) //expected primitives.size == 1
            {
                tinygltf::Primitive primitive = mesh.primitives[i];
                tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
                auto rng = txMap.equal_range(primitive.material);
                for(auto& v = rng.first; v != rng.second; ++v)
                    mat_ptr.emplace_back(&v->second);
                index_sz = indexAccessor.count;
                for(auto& attrib: primitive.attributes)
                {
                    tinygltf::Accessor accessor = model.accessors[attrib.second];
                    int byteStride =
                            accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                    Console::fmt_log_skip_uniq("primitive attributes {}", Console::info, attrib.first);
                    int vaa = -1;
                    if(attrib.first.compare("POSITION") == 0)
                        vaa = 0;
                    if(attrib.first.compare("NORMAL") == 0)
                        vaa = 3;
                    if(attrib.first.compare("TEXCOORD_0") == 0)
                        vaa = 2;
                    int size = 1;

                    if(accessor.type != TINYGLTF_TYPE_SCALAR)
                    {
                        size = accessor.type;
                    }
                    if(vaa > -1)
                    {
                        VAOBindMap::vao_data data{
                                .vbo_id = accessor.bufferView,
                                .type = accessor.componentType,
                                .offset = buff_offset(accessor.byteOffset),
                                .stride = byteStride,
                                .normilized = accessor.normalized,
                                .size_type = size};
                        vao_map.add(vaa, data);
                        Console::fmt_log_skip_uniq("add vao map to bind point {}: vbo_id {}, type {}, offset {}, stride {}, normilized {}", Console::info,
                                                   vaa, data.vbo_id, data.type, data.offset, data.stride, data.normilized);
                    } else
                    {
                        Console::addLog("vao missing", Console::error);
                    }
                }
            }
            return Mesh{std::move(raw_buffer), vao_map, index_sz, mat_ptr};
            // vao.unbind();
        }

//        VertexArrayObject setVAO()
//        {
//            VertexArrayObject v;
//            v.create(1);
//            v.bind();
//            std::map<int, VertexBufferObject> vbos;
//            Console::fmt_log_skip_uniq("create vbo from vbo_map and bind to vao", Console::info);
//
//            for(auto &[id, buffer]: raw_buffer)
//            {
//                VertexBufferObject vbo(buffer.first);
//                vbo.create(buffer.second.size());
//                vbo.setData(buffer.second.size(), &buffer.second.at(0));
//                vbos.emplace(std::make_pair(id, std::move(vbo)));
//            }
//            for(auto &[point, vao_]: vao_map)
//                v.setPointer_u(vbos[vao_.vbo_id], point, vao_.size_type,
//                               vao_.type, vao_.stride, vao_.offset, vao_.normilized);
//
//            v.unbind();
//            return v;
//        }

        void bindModelNodes(tinygltf::Model& model,
                            tinygltf::Node& node)
        {
            if(node.rotation.size() >= 3 && node.translation.size() >= 3)
                Console::fmt_log("node:{}\nrotation: x.{} y.{} z.{} translation: x.{} y.{} z.{}", Console::info,
                                 node.name,
                                 node.rotation[0],
                                 node.rotation[1],
                                 node.rotation[2],
                                 node.translation[0],
                                 node.translation[1],
                                 node.translation[2]);
            if((node.mesh >= 0) && (node.mesh < model.meshes.size()))
            {
                meshes.emplace_back(bindMesh(model, model.meshes[node.mesh]));
            }

            for(size_t i = 0; i < node.children.size(); i++)
            {
                assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
                bindModelNodes(model, model.nodes[node.children[i]]);
            }
        }

        Model bindModel(tinygltf::Model& model)
        {
            const tinygltf::Scene& scene = model.scenes[model.defaultScene];

            std::map<int, std::string> images;
            int index = 0;
            for(auto& img: model.images)
            {
                images.emplace(index, img.uri);
                index++;
            }
            index = 0;
            std::map<int, int> textures;
            for(auto& tex: model.textures)
            {
                textures.emplace(index, tex.source);
                index++;
            }
            index = 0;
            for(auto& mat: model.materials)
            {
                _Material mater(mat.name);
                if(int tIndex = mat.pbrMetallicRoughness.baseColorTexture.index; tIndex >= 0)
                {
                    mater.textureMap.emplace(Material::Type::TEXTURE_ALBEDO, images[textures[tIndex]]);
                }
                if(int tIndex = mat.pbrMetallicRoughness.metallicRoughnessTexture.index; tIndex >= 0)
                {
                    mater.textureMap.emplace(Material::Type::TEXTURE_METALLIC, images[textures[tIndex]]);
                    mater.textureMap.emplace(Material::Type::TEXTURE_ROUGHNESS, images[textures[tIndex]]);
                }
                if(int tIndex = mat.emissiveTexture.index; tIndex >= 0)
                {
                    mater.textureMap.emplace(Material::Type::TEXTURE_EMISSIVE, images[textures[tIndex]]);
                }
                if(int tIndex = mat.normalTexture.index; tIndex >= 0)
                {
                    mater.textureMap.emplace(Material::Type::TEXTURE_NORMAL, images[textures[tIndex]]);
                }
                if(int tIndex = mat.occlusionTexture.index; tIndex >= 0)
                {
                    mater.textureMap.emplace(Material::Type::TEXTURE_OCCLUSION, images[textures[tIndex]]);
                }
                mat_map.materials.emplace(index, mater);
                index++;
            }

            for(auto& mat: mat_map.materials)
            {
                Console::fmt_log_skip_uniq("Material {}", Console::info, mat.second.name);
                for(auto& txt: mat.second.textureMap)
                {
                    Texture t;
                    t.LoadFromFile("Data//model//glTF//" + txt.second);
                    //tx.emplace_back(std::move(t));
                    txMap.emplace(std::make_pair(mat.first, Material{std::move(t), txt.first}));
                    //  Console::fmt_log_skip_uniq("texture {}\npath: {}", Console::info, txt.first, txt.second);
                }
            }
            for(size_t i = 0; i < scene.nodes.size(); ++i)
            {
                assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
                bindModelNodes(model, model.nodes[scene.nodes[i]]);
            }
            Model m;
            m.meshes = std::move(meshes);
            m.materials = std::move(txMap);
            return m;
        }
    };

    class assimp_loader
    {
    private:
        // Texture txt;
        std::multimap<int, Material> materials;
        VertexArray model_data;
        std::string path;
        int insize;
        std::vector<Mesh> meshes;

    public:
        assimp_loader() //: indecesVbo(BufferObject::BufferType::EAB)
        {
        }

        assimp_loader(std::string_view path) : path(path) //, indecesVbo(BufferObject::BufferType::EAB)
        {
        }

        Model load(std::string_view path)
        {
            // txt.LoadFromFile("Data//model//diffuse.jpg");
            this->path = path;
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(this->path,
                                                     aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Console::addLog("ASSIMP LOAD ERROR", Console::error);
                //return;
            }
            loadMaterialTextures(scene);
            processNode(scene->mRootNode, scene);
            Model m;
            m.setMaterials(std::move(materials));
            m.meshes = std::move(meshes);
            m.init();
            return m;
        }

        void processNode(aiNode* node, const aiScene* scene)
        {
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                processMesh(mesh, scene);
            }
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        void processMesh(const aiMesh* mesh, const aiScene* scene)
        {
            RawBufferStorage raw_storage;
            VAOBindMap bind_map;
            palka::VertArray array;
            raw_storage.setData(1, {.type = BufferObject::VBO, .data = {}, .bytesAdded = 0});
            raw_storage.setData(2, {.type = BufferObject::VBO, .data = {}, .bytesAdded = 0});
            raw_storage.setData(3, {.type = BufferObject::VBO, .data = {}, .bytesAdded = 0});
            raw_storage.setData(4, {.type = BufferObject::EAB, .data = {}, .bytesAdded = 0});
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vec3f vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                raw_storage.addData(1, vector);
                array.add(vector);

                if(mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    array[i].normal = vector;
                    raw_storage.addData(2, vector);
                }
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    Vec2f vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    raw_storage.addData(3, vec);
                    array[i].texCoord = vec;
                } else
                {
                    raw_storage.addData(3, glm::vec2(0.0f, 0.0f));
                    array[i].texCoord = glm::vec2(0.0f, 0.0f);
                }
            }

            std::vector<GLuint> arr;
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                {
                    raw_storage.addData(4, face.mIndices[j]);
                    arr.push_back(face.mIndices[j]);
                }
            }

            insize = arr.size();
//            layout (location = 0) in vec3 aPos;
//            layout (location = 1) in vec4 vertColor;
//            layout (location = 2) in vec2 aTexCoord;
//            layout (location = 3) in vec3 aNormal;

            bind_map.add(0, VAOBindMap::vao_data{
                    .vbo_id = 1,
                    .type = GL_FLOAT,
                    .offset = 0,
                    .stride = sizeof(Vec3f),
                    .normilized = false,
                    .size_type = 3});

            bind_map.add(2, VAOBindMap::vao_data{
                    .vbo_id = 3,
                    .type = GL_FLOAT,
                    .offset = 0,
                    .stride = sizeof(Vec2f),
                    .normilized = false,
                    .size_type = 2});

            bind_map.add(3, VAOBindMap::vao_data{
                    .vbo_id = 2,
                    .type = GL_FLOAT,
                    .offset = 0,
                    .stride = sizeof(Vec3f),
                    .normilized = false,
                    .size_type = 3});

            std::vector<Material::Ptr> mats = setMaterialPointers(mesh->mMaterialIndex);

            meshes.emplace_back(std::move(raw_storage), bind_map, insize, mats);
        }

        std::vector<Material::Ptr> setMaterialPointers(int matIndex)
        {
            std::vector<Material::Ptr> ptr;
            auto range = materials.equal_range(matIndex);
            for(auto i = range.first; i != range.second; ++i)
            {
                ptr.emplace_back(&i->second);
            }
            return ptr;
        }

        void getTexture(aiMaterial* material, aiTextureType type, Material::Type typ, int index)
        {
            if(material->GetTextureCount(type) > 0)
            {
                aiString str;
                material->GetTexture(type, 0, &str);
                materials.emplace(std::make_pair(index, Material{Texture{"Data//model//" + std::string(str.C_Str())}, typ}));
            }
        }

        void loadMaterialTextures(const aiScene* scene) //(aiMaterial* mat, aiTextureType type, std::string typ)
        {
            if(scene->HasMaterials())
                for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
                {
                    aiMaterial* material = scene->mMaterials[i];
                    getTexture(material, aiTextureType_SHININESS, Material::Type::TEXTURE_ROUGHNESS, i);
                    getTexture(material, aiTextureType_DIFFUSE, Material::Type::TEXTURE_ALBEDO, i);
                    getTexture(material, aiTextureType_SPECULAR, Material::Type::TEXTURE_METALLIC, i);
                    getTexture(material, aiTextureType_NORMALS, Material::Type::TEXTURE_NORMAL, i);
                }
            //            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            //            {
            //                aiString str;
            //                mat->GetTexture(type, i, &str);
            //                if(auto it = std::ranges::find_if(materials, [str](const Material& material)
            //                    { return material.texture.getFilePath() == str.C_Str(); }); it == materials.end()
            //                        )
            //                    materials.emplace_back(Texture{str.C_Str()}, typ);
            //
            //            }
        }

//        void render(ShaderProgram& shader)
//        {
//            m.render(shader);
////            for(auto& m: models)
////                m.render(shader);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glActiveTexture(GL_TEXTURE0 + 1);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glActiveTexture(GL_TEXTURE0 + 2);
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
    };
}

#endif //PALKA_MESH_H
