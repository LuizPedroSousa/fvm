#include "project-manager.hpp"
#include "assert.hpp"

namespace astralix {
  ProjectID ProjectManager::get_active_project_id() const {
    return m_active_project_id;
  }

  Ref<Project> ProjectManager::get_active_project() {
    auto it = m_projects.find(m_active_project_id);

    if (it != m_projects.end()) {
      return it->second;
    }

    return nullptr;
  }

  std::vector<Ref<Project>> ProjectManager::get_projects() const {
    std::vector<Ref<Project>> result;

    for (const auto& pair : m_projects) {
      result.push_back(pair.second);
    }

    return result;
  }

  Ref<Project> ProjectManager::add_project(Ref<Project> project) {
    auto project_id = project->get_project_id();
    auto created_project =
      m_projects.emplace(project->get_project_id(), std::move(project));

    ASTRA_ENSURE(!created_project.second, "Error creating new Project!");

    m_active_project_id = project_id;

    return project;
  };

} // namespace astralix
