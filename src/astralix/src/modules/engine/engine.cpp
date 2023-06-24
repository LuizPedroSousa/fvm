#include "engine.hpp"
#include "systems/layer-system.hpp"
#include "systems/physics-system.hpp"
#include "systems/render-system.hpp"
#include "systems/scene-system.hpp"
#include "time.hpp"

namespace astralix {
Engine *Engine::m_instance = nullptr;

void Engine::init() { m_instance = new Engine; }

Engine::~Engine() {
  delete m_system_manager;
  delete m_entity_manager;
  delete m_component_manager;
  delete m_resource_manager;
  delete m_render_configuration;
};

void Engine::end() { delete m_instance; }

Engine::Engine()
    : m_component_manager(new ComponentManager()),
      m_system_manager(new SystemManager()) {
  m_entity_manager = new EntityManager(m_component_manager);
  m_resource_manager = new ResourceManager();
  m_render_configuration =
      new RenderConfiguration({.samples = 4, .is_enabled = true});
}

void Engine::start() {
  m_system_manager->add_system<SceneSystem>();
  m_system_manager->add_system<RenderSystem>();
  m_system_manager->add_system<PhysicsSystem>();
  m_system_manager->add_system<LayerSystem>();

  m_system_manager->start();
}

void Engine::update() {
  m_system_manager->update(Time::get()->get_deltatime());
}

} // namespace astralix
