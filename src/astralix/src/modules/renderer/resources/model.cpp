#include "model.hpp"
#include "glad/glad.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "engine.hpp"

namespace astralix {

Either<BaseException, Model> Model::create(ResourceID id,
                                           const char *filename) {

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      get_path(filename),
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

  ASSERT(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE,
         importer.GetErrorString());

  std::vector<Mesh> meshes;
  std::vector<ResourceID> materials;

  auto has_node_processed =
      process_nodes(scene->mRootNode, scene, meshes, materials);

  ASSERT_COMPARE_THROW(has_node_processed);

  return Model(id, filename, meshes, materials);
};

Either<BaseException, Unit>
Model::process_nodes(const aiNode *current_node, const aiScene *scene,
                     std::vector<Mesh> &meshes,
                     std::vector<ResourceID> &materials) {
  for (u_int i = 0; i < current_node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[current_node->mMeshes[i]];

    auto processed_mesh = process_mesh(mesh, scene, materials);
    ASSERT_COMPARE(processed_mesh);

    meshes.push_back(processed_mesh.right());
  }

  for (u_int i = 0; i < current_node->mNumChildren; i++) {
    process_nodes(current_node->mChildren[i], scene, meshes, materials);
  }

  return Unit();
}

Either<BaseException, Mesh>
Model::process_mesh(aiMesh *node_mesh, const aiScene *scene,
                    std::vector<ResourceID> &materials) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  // get vertices
  for (u_int i = 0; i < node_mesh->mNumVertices; i++) {
    Vertex vertex;

    vertex.position =
        glm::vec3(node_mesh->mVertices[i].x, node_mesh->mVertices[i].y,
                  node_mesh->mVertices[i].z);

    if (node_mesh->HasNormals()) {
      vertex.normal =
          glm::vec3(node_mesh->mNormals[i].x, node_mesh->mNormals[i].y,
                    node_mesh->mNormals[i].z);
    }

    if (node_mesh->mTextureCoords[0]) {
      vertex.texture_cordinates = glm::vec2(node_mesh->mTextureCoords[0][i].x,
                                            node_mesh->mTextureCoords[0][i].y);
    } else {
      vertex.texture_cordinates = glm::vec2(0);
    }

    vertices.push_back(vertex);
  };

  for (unsigned int i = 0; i < node_mesh->mNumFaces; i++) {
    aiFace face = node_mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (node_mesh->mMaterialIndex > 0) {
    aiMaterial *ai_material = scene->mMaterials[node_mesh->mMaterialIndex];
    if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0 ||
        ai_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {

      auto resource_manager = Engine::get()->get_resource_manager();

      std::string name = ai_material->GetName().C_Str();

      std::string id = "materials::" + name;
      auto material = resource_manager->load_material(id, ai_material);
      materials.push_back(material->get_resource_id());
    }
  }

  return Mesh(vertices, indices);
}

std::filesystem::path Model::get_path(const char *filename) {
  auto base_path = std::filesystem::current_path()
                       .parent_path()
                       .append("src/assets/models")
                       .append(filename);

  return base_path;
};

} // namespace astralix