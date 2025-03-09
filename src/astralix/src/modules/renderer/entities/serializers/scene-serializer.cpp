#include "scene-serializer.hpp"
#include "context-proxy.hpp"
#include "entities/object.hpp"
#include "log.hpp"
#include "managers/entity-manager.hpp"
#include "managers/project-manager.hpp"
#include "string"

namespace astralix {

SceneSerializer::SceneSerializer(Ref<Scene> scene) { m_scene = scene; }

SceneSerializer::SceneSerializer() {}

void SceneSerializer::serialize() {
  SerializationContext &ctx = *m_ctx.get();

  auto entity_manager = EntityManager::get();
  auto project_manager = ProjectManager::get();

  auto entities = entity_manager->get_entities<Object>();

  for (int i = 0; i < entities.size(); i++) {
    auto &entity = entities.at(i);

    ctx["entities"][i]["active"] = entity->is_active();
    ctx["entities"][i]["id"] = (std::string)entity->get_entity_id();
    ctx["entities"][i]["name"] = entity->name;

    auto components = entity->get_components();

    for (int j = 0; j < components.size(); j++) {
      auto serializer = components[j]->get_serializer();

      if (serializer != nullptr) {
        serializer->serialize();
      }

      ctx["entities"][i]["components"][j] = serializer->get_ctx();
    }
  }
}

// void SceneSerializer::save() {
//   auto project_manager = ProjectManager::get();
//
//   auto active_project = project_manager->get_active_project();
//
//   auto out_path =
//   std::filesystem::path(active_project->get_config().directory)
//                       .append("scenes")
//                       .append((std::string)m_scene->get_name() +
//                       "_meta.json");
//
//   auto root = serialize();
//   write(out_path, root);
// }

void SceneSerializer::deserialize() {}
} // namespace astralix
