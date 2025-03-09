#pragma once
#include "assimp/scene.h"
#include "components/component.hpp"
#include "guid.hpp"
#include "functional"
#include "glm/glm.hpp"
#include "renderer-api.hpp"
#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "vector"
#include "vertex-buffer.hpp"

namespace astralix {

  class MeshComponent : public Component<MeshComponent> {

  public:
    MeshComponent(COMPONENT_INIT_PARAMS);

    void start();
    void update();

    std::vector<Mesh> get_meshes() { return m_meshes; }

    void change_draw_type(RendererAPI::DrawPrimitive primitive_type) {
      for (auto& mesh : m_meshes) {
        mesh.draw_type = primitive_type;
      }
    }

    void attach_meshes(std::vector<Mesh> meshes) {
      std::size_t seed = 0;

      m_meshes.insert(m_meshes.end(), meshes.begin(), meshes.end());
    };

    void attach_mesh(Mesh mesh) { m_meshes.push_back(mesh); };

    void set_draw_type(VertexBuffer::DrawType draw_type) {
      m_draw_type = draw_type;
    };

  private:
    std::vector<Mesh> m_meshes;
    VertexBuffer::DrawType m_draw_type;
  };

} // namespace astralix
