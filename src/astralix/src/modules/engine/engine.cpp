#include "engine.hpp"
#include "managers/project-manager.hpp"
#include "systems/physics-system.hpp"
#include "systems/render-system.hpp"
#include "systems/scene-system.hpp"
#include "time.hpp"

namespace astralix {
  Engine* Engine::m_instance = nullptr;

  void Engine::init() {
    if (m_instance == nullptr) {
      m_instance = new Engine;
    }
  }

  void Engine::end() { delete m_instance; }

  Engine::Engine() {
    this->msaa = { .samples = 4, .is_enabled = false };

    EntityManager::init();
    ComponentManager::init();

    ProjectManager::init();
    SceneManager::init();
    ResourceManager::init();

    this->renderer_api = std::move(RendererAPI::create(RendererAPI::API::OpenGL));
  }

  void Engine::start() {
    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.attachments = { FramebufferTextureFormat::RGBA8,
                                    FramebufferTextureFormat::RED_INTEGER,
                                    FramebufferTextureFormat::Depth };
    framebuffer_spec.width = 1920;
    framebuffer_spec.height = 1080;
    framebuffer = std::move(Framebuffer::create(framebuffer_spec));

    auto system_manager = SystemManager::get();

    system_manager->add_system<SceneSystem>();
    system_manager->add_system<RenderSystem>();
    system_manager->add_system<PhysicsSystem>();
  }

  void Engine::update() {
  }

} // namespace astralix
