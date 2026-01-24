#pragma once
#include "arena.hpp"
#include "assert.hpp"
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

  SerializationFormat formatFromString(const std::string &format) {
    static const std::map<std::string, SerializationFormat> formats = {
        {"json", SerializationFormat::Json},
    };

    auto it = formats.find(format);

    ASTRA_ENSURE_WITH_SUGGESTIONS(it == formats.end(), formats, format,
                                  "serialization format",
                                  "ProjectSerialization")

    return it->second;
  }
};

struct ProjectResourceConfig {
  std::string directory;
};

struct WindowConfig {
  std::string title;
  int height;
  int width;
};

struct SystemConfig {
  std::string title;
  int height;
  int width;
};

struct ProjectConfig {
  std::string name = "Untitled";
  std::string directory;

  std::string manifest;

  std::vector<WindowConfig> windows;

  std::vector<SystemConfig> systems;

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
