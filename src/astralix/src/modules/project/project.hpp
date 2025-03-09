#pragma once
#include "arena.hpp"
#include "base.hpp"
#include "guid.hpp"
#include "serialization-context.hpp"
#include "serializers/project-serializer.hpp"
#include <string>

namespace astralix {

class ProjectSerializer;

#define RESOURCE_INIT_PARAMS const ResourceID &id

struct ProjectSerializationConfig {
  SerializationFormat format;
};

struct ProjectResourceConfig {
  std::string directory;
};

struct ProjectConfig {
  std::string name = "Untitled";
  std::string directory;

  ProjectResourceConfig resources;
  ProjectSerializationConfig serialization;
};

class Project {
public:
  ProjectID get_project_id() const { return m_project_id; }

  static Ref<Project> create(ProjectConfig config);
  ProjectConfig &get_config() { return m_config; }
  void save(ElasticArena &arena);
  void load(ElasticArena &arena);
  Project(ProjectConfig config);

private:
  ProjectConfig m_config;
  ProjectID m_project_id;
  Scope<ProjectSerializer> m_serializer;
};

} // namespace astralix
