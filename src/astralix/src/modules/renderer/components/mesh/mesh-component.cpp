#include "mesh-component.hpp"
#include "components/material/material-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"
#include "vector"

namespace astralix {

MeshComponent::MeshComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(MeshComponent){};

void MeshComponent::start() {
  for (int i = 0; i < m_meshes.size(); i++) {
    create_buffers(&m_meshes[i]);
  }
}

void MeshComponent::update() {

  for (int i = 0; i < m_meshes.size(); i++) {
    glBindVertexArray(m_meshes[i].m_buffers.VAO);
    glDrawElements(GL_TRIANGLES,
                   static_cast<unsigned int>(m_meshes[i].indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

void MeshComponent::create_buffers(Mesh *mesh) {
  u_int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex),
               &mesh->vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0],
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texture_cordinates));

  unbind_buffer();

  mesh->m_buffers = {.VAO = VAO, .VBO = VBO, .EBO = EBO};
};

void MeshComponent::bind_buffer(u_int *buffer) {}

void MeshComponent::unbind_buffer() { glBindVertexArray(0); }

} // namespace astralix