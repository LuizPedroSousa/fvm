#pragma once
#include "base.hpp"
#include "ecs/guid.hpp"
#include "serializers/project-serializer.hpp"
#include <string>

namespace astralix {

class ProjectSerializer;

#define RESOURCE_INIT_PARAMS const ResourceID &id

struct ProjectResourceConfig {
  std::string directory;
};

struct ProjectConfig {
  std::string name = "Untitled";
  std::string directory;

  ProjectResourceConfig resources;
};

class Project {
public:
  ProjectID get_project_id() const { return m_project_id; }

  static Ref<Project> create(ProjectConfig config);
  ProjectConfig &get_config() { return m_config; }
  void save();
  void load();
  Project(ProjectConfig config);

private:
  ProjectConfig m_config;
  ProjectID m_project_id;
  Scope<ProjectSerializer> m_serializer;
};

} // namespace astralix
