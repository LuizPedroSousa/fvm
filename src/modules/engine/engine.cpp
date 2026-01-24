#include "engine.hpp"
#include "managers/entity-manager.hpp"
#include "managers/project-manager.hpp"
#include "managers/resource-manager.hpp"
#include "managers/scene-manager.hpp"
#include "systems/physics-system.hpp"
#include "systems/render-system/render-system.hpp"
#include "systems/scene-system.hpp"
#include "window.hpp"

namespace astralix {
Engine *Engine::m_instance = nullptr;

void Engine::init() {
  if (m_instance == nullptr) {
    m_instance = new Engine;
  }
}

void Engine::end() { delete m_instance; }

Engine::Engine() {
  this->msaa = {.samples = 4, .is_enabled = false};

  EntityManager::init();
  ComponentManager::init();

  ProjectManager::init();
  SceneManager::init();
  ResourceManager::init();

  this->renderer_api = std::move(RendererAPI::create(RendererAPI::API::OpenGL));
}

void Engine::start() {
  FramebufferSpecification framebuffer_spec;
  framebuffer_spec.attachments = {
      FramebufferTextureFormat::RGBA32F, FramebufferTextureFormat::RGBA32F,
      FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};

  auto window = Window::get();

  framebuffer_spec.width = window->get_width();
  framebuffer_spec.height = window->get_height();

  framebuffer = std::move(Framebuffer::create(framebuffer_spec));

  auto system_manager = SystemManager::get();

  system_manager->add_system<SceneSystem>();
  system_manager->add_system<RenderSystem>();
  system_manager->add_system<PhysicsSystem>();
}

void Engine::update() {}

} // namespace astralix
