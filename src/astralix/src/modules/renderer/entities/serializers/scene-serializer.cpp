#include "scene-serializer.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "entities/object.hpp"
#include "managers/project-manager.hpp"
#include "string"

namespace astralix {
  SceneSerializer::SceneSerializer() {}
  SceneSerializer::SceneSerializer(Ref<Scene> scene) : m_scene(scene) {}

  Json::Value SceneSerializer::parse() {
    Json::Value root;

    auto entity_manager = EntityManager::get();
    auto project_manager = ProjectManager::get();

    auto entities = entity_manager->get_entities<Object>();

    for (int i = 0; i < entities.size(); i++) {
      auto& entity = entities.at(i);
      root["entities"][i]["active"] = entity->is_active();
      root["entities"][i]["id"] = (std::string)entity->get_entity_id();
      root["entities"][i]["name"] = entity->name;
      auto components = entity->get_components();

      Json::Value parsed_components;
      for (int j = 0; j < components.size(); j++) {

        auto parsed_component = components[j]->get_serializer()->parse();

        parsed_components.append(parsed_component);
      }

      root["entities"][i]["components"] = parsed_components;
    }

    return root;
  }

  void SceneSerializer::serialize() {
    auto project_manager = ProjectManager::get();

    auto active_project = project_manager->get_active_project();

    auto out_path = std::filesystem::path(active_project->get_config().directory)
      .append("scenes")
      .append((std::string)m_scene->get_name() + "_meta.json");

    auto root = parse();
    write(out_path, root);
  }

  void SceneSerializer::deserialize() {}
} // namespace astralix