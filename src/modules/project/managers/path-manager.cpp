#include "path-manager.hpp"
#include "assert.hpp"
#include "log.hpp"
#include "managers/project-manager.hpp"

namespace astralix {
constexpr std::string_view engineAssetsDirectory = ASTRALIX_ASSETS_DIR;

std::filesystem::path
PathManager::resolve_project_path(std::string relative_path) {
  auto project = ProjectManager::get()->get_active_project();

  auto base_path =
      std::filesystem::path(project->get_config().resources.directory)
          .append(relative_path);

  return base_path;
}

std::filesystem::path
PathManager::resolve_engine_path(std::string relative_path) {

  auto base_path =
      std::filesystem::path(engineAssetsDirectory).append(relative_path);

  return base_path;
}

std::filesystem::path PathManager::resolve(Ref<Path> path) {

  switch (path->get_base_directory()) {
  case BaseDirectory::Project: {
    LOG_DEBUG("PROJECT PATH", path->get_relative_path());
    return resolve_project_path(path->get_relative_path());
  }

  case BaseDirectory::Engine: {
    LOG_DEBUG("ENGINE PATH", path->get_relative_path());
    return resolve_engine_path(path->get_relative_path());
  }

  default: {
    ASTRA_EXCEPTION(true, std::string("Can't resolve relative path basis"))
  }
  }
};

} // namespace astralix
