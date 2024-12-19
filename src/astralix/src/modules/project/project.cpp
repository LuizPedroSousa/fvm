#include "project.hpp"
#include "ecs/guid.hpp"

namespace astralix {

Project::Project(ProjectConfig config)
    : m_config(config), m_project_id(ProjectID()) {}

Ref<Project> Project::create(ProjectConfig config) {
  auto project = create_ref<Project>(config);

  project->m_serializer = create_scope<ProjectSerializer>(project);

  return project;
}

void Project::save() { m_serializer->serialize(); }
void Project::load() { m_serializer->deserialize(); }

} // namespace astralix
