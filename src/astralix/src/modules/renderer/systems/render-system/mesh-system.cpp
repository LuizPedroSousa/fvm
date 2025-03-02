#include "mesh-system.hpp"

#include "base.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/guid.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "glad//glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "log.hpp"
#include "managers/resource-manager.hpp"
#include "renderer-api.hpp"
#include "storage-buffer.hpp"
#include "trace.hpp"
#include <cmath>
#include <cstdint>

#include <unordered_map>

namespace astralix {

MeshSystem::MeshSystem() {}

void MeshSystem::pre_update(double dt) {}

void MeshSystem::fixed_update(double fixed_dt) {}

void MeshSystem::start() {
  auto resource_manager = ResourceManager::get();

  m_storage_buffer = StorageBuffer::create(1000 * 1000 * sizeof(glm::mat4));
}

#define GET_ENTITY(T) m_entity_manager->get_entity<T>();

void MeshSystem::update(double dt) {
  CHECK_ACTIVE(this);
  ASTRA_PROFILE_N("MeshSystem Update");

  auto entity_manager = EntityManager::get();

  auto objects = entity_manager->get_entities<Object>();

  for (auto object : objects) {
    ASTRA_PROFILE_N("MeshSystem Object Loop");

    auto mesh_component = object->get_component<MeshComponent>();

    if (mesh_component == nullptr) {
      continue;
    }

    auto meshes = mesh_component->get_meshes();

    auto batch_id = compute_batch_id(meshes);

    auto batch_exists = m_batches.find(batch_id);

    auto transform = object->get_component<TransformComponent>();

    auto entity_id = object->get_entity_id();

    if (batch_exists == m_batches.end()) {
      std::unordered_map<EntityID, glm::mat4> transforms;

      transforms[entity_id] = transform->matrix;
      m_batches[batch_id] = {.meshes = meshes, .transforms = transforms};

      continue;
    }

    if (transform->matrix != batch_exists->second.transforms[entity_id]) {
      batch_exists->second.transforms[entity_id] = transform->matrix;
      batch_exists->second.is_dirty = true;
      continue;
    }
    batch_exists->second.is_dirty = false;
  }

  std::vector<Mesh> indexed;

  for (auto &[_, batch] : m_batches) {
    if (!batch.is_dirty) {
      continue;
    }

    if (batch.transforms.size() == 1) {
      for (auto mesh : batch.meshes) {
        indexed.push_back(mesh);
      }

      continue;
    }

    m_storage_buffer->bind();

    std::vector<glm::mat4> transforms;

    for (const auto &[_, transform] : batch.transforms) {
      transforms.push_back(transform);
    }

    m_storage_buffer->set_data(transforms.data(),
                               transforms.size() * sizeof(glm::mat4));

    m_storage_buffer->unbind();
  }

  auto engine = Engine::get();

  for (auto batch : m_batches) {
    for (auto mesh : batch.second.meshes) {
      auto vertex_array = mesh.vertex_array;
      vertex_array->bind();

      m_storage_buffer->bind();

      uint32_t count = vertex_array->get_index_buffer()->get_count();

      engine->renderer_api->draw_instanced_indexed(
          RendererAPI::DrawPrimitive::TRIANGLES, count,
          batch.second.transforms.size());

      vertex_array->unbind();
    }
  }

  for (auto mesh : indexed) {
    auto vertex_array = mesh.vertex_array;
    vertex_array->bind();

    engine->renderer_api->draw_indexed(vertex_array,
                                       RendererAPI::DrawPrimitive::TRIANGLES);

    vertex_array->unbind();
  }
}

MeshBatchID MeshSystem::compute_batch_id(std::vector<Mesh> &meshes) {
  ASTRA_PROFILE_N("MeshSystem Compute Batch ID");

  MeshBatchID batch_id;

  for (auto mesh : meshes) {
    batch_id ^= mesh.id;
  }

  return batch_id;
}

MeshSystem::~MeshSystem() {}

} // namespace astralix
