#pragma once
#include "assimp/scene.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "mesh.hpp"
#include "resources/resource.hpp"
#include "vector"

class Model : public Resource {
public:
  Model(RESOURCE_INIT_PARAMS, const char *filename, std::vector<Mesh> meshes,
        std::vector<ResourceID> materials)
      : RESOURCE_INIT(), filename(filename), meshes(meshes),
        materials(materials){};

  Model(){};

  static Either<BaseException, Model> create(ResourceID id,
                                             const char *filename);

  const char *filename;
  std::vector<Mesh> meshes;
  std::vector<ResourceID> materials;

private:
  static Either<BaseException, Unit>
  process_nodes(const aiNode *first_node, const aiScene *scene,
                std::vector<Mesh> &meshes, std::vector<ResourceID> &materials);
  static Either<BaseException, Mesh>
  process_mesh(aiMesh *mesh, const aiScene *scene,
               std::vector<ResourceID> &materials);
  static std::filesystem::path get_path(const char *filename);
};