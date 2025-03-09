#include "game.hpp"
#include "managers/project-manager.hpp"
#include "managers/scene-manager.hpp"
#include "project.hpp"
#include "scenes/prologue.hpp"

Game::Game() {}

void Game::start() {

  auto dir = std::filesystem::current_path().parent_path().append("src").append(
      "game");

  auto project = astralix::Project::create(
      {.name = "Game",
       .directory = dir,
       .resources = {.directory = dir.append("assets")}

      });
  // project->save();
  // project->load();

  astralix::ProjectManager::get()->add_project(project);

  astralix::SceneManager::get()->add_scene<Prologue>();
}
