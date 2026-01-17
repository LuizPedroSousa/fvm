#pragma once
#include "assimp/scene.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "mesh.hpp"
#include "resources/material.hpp"
#include "resources/resource.hpp"
#include "vector"

namespace astralix {

class Model : public Resource {
public:
  Model(RESOURCE_INIT_PARAMS, const char *path, std::vector<Mesh> meshes,
        std::vector<ResourceID> materials)
      : RESOURCE_INIT(), path(path), meshes(meshes), materials(materials) {};

  Model() {};

  static Ref<Model> create(ResourceID id, const char *path);

  const char *path;
  std::vector<Mesh> meshes;
  std::vector<ResourceID> materials;

private:
  static void process_nodes(const aiNode *first_node, const aiScene *scene,
                            std::vector<Mesh> &meshes,
                            std::vector<ResourceID> &materials,
                            std::filesystem::path path);
  static Mesh process_mesh(aiMesh *mesh, const aiScene *scene,
                           std::vector<ResourceID> &materials,
                           std::filesystem::path path);
  static std::filesystem::path get_path(const char *filename);

  static Ref<Material> load_material(ResourceID material_id,
                                     aiMaterial *ai_material,
                                     std::filesystem::path path);
};

} // namespace astralix
