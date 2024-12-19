#include "project-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  ProjectSerializer::ProjectSerializer(Ref<Project> project)
    : m_project(project) {
  }

  ProjectSerializer::ProjectSerializer() {}

  void ProjectSerializer::serialize() {
    Json::Value root = parse();

    auto& config = m_project->get_config();

    auto path =
      std::filesystem::path(config.directory).append("project_meta.json");

    write(path, root);
  }

  Json::Value ProjectSerializer::parse() {
    Json::Value root;

    auto& config = m_project->get_config();
    root["config"]["name"] = config.name;
    root["config"]["directory"] = config.directory;
    root["config"]["resources"]["directory"] = config.resources.directory;

    return root;
  }

  void ProjectSerializer::deserialize() {
    auto& config = m_project->get_config();

    auto data = read(config.directory);

    config.name = data["config"]["name"].as<std::string>();
    config.directory = data["config"]["directory"].as<std::string>();
    config.resources.directory =
      data["config"]["resourcess"]["directory"].as<std::string>();
  }

} // namespace astralix
