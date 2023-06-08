#include "game.hpp"
#include "application.hpp"
#include "scene-system.hpp"
#include "systems/layer-system.hpp"
#include "systems/render-system.hpp"

Game *Game::m_instance = nullptr;

void Game::init() { m_instance = new Game; }

Game::Game()
    : m_component_manager(new ComponentManager()),
      m_system_manager(new SystemManager()) {
  m_entity_manager = new EntityManager(m_component_manager);
  m_resource_manager = new ResourceManager();
}

Either<BaseException, Unit> Game::start() {
  auto scene_system = m_system_manager->add_system<SceneSystem>();
  m_system_manager->add_system<RenderSystem>(scene_system);
  m_system_manager->add_system<LayerSystem>();

  auto has_started = m_system_manager->start();

  ASSERT_COMPARE(has_started);

  return Unit();
}

void Game::update() { m_system_manager->update(Application::get_deltatime()); }
