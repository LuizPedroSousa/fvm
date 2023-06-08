#pragma once
#include "assimp/scene.h"
#include "base/component.hpp"
#include "functional"
#include "glm/glm.hpp"
#include "resources/mesh.hpp"
#include "resources/texture-renderer.hpp"
#include "resources/texture.hpp"
#include "utils/guid.hpp"
#include "vector"

class MeshComponent : public Component<MeshComponent> {
  public:
  MeshComponent(COMPONENT_INIT_PARAMS);

  void start();
  void draw();

  std::vector<Mesh> *get_meshes() {
    return &m_meshes;
  }

  void attach_meshes(std::vector<Mesh> meshes) {
    m_meshes.insert(m_meshes.end(), meshes.begin(), meshes.end());
  };

  void attach_mesh(Mesh mesh) {
    m_meshes.push_back(mesh);
  };

  private:
  void
  create_buffers(Mesh *mesh);
  void bind_buffer(u_int *buffer);
  void unbind_buffer();

  std::vector<Mesh> m_meshes;
};