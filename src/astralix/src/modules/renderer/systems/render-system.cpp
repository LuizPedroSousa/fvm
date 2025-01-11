#include "render-system.hpp"
#include "glad/glad.h"

#include "engine.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "iostream"
#include "window.hpp"
#include <iostream>
#include <iterator>
#include <sys/mman.h>
#include "websocket-server.hpp"

#define SHM_NAME "/framebuffer_shm"
#define WIDTH 1920
#define HEIGHT 1080
#define PIXEL_SIZE 4 // RGBA

void* ptr = nullptr;
int fd = -1;

// The io_context is required for all I/O

namespace astralix {


  std::string base64_encode(const std::vector<unsigned char>& data) {
    static const char base64_table[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0;
    int valb = -6;

#pragma omp parallel for
    for (unsigned char c : data) {
      val = (val << 8) + c;
      valb += 8;
      while (valb >= 0) {
        result.push_back(base64_table[(val >> valb) & 0x3F]);
        valb -= 6;
      }
    }

    if (valb > -6) {
      result.push_back(base64_table[((val << 8) >> (valb + 8)) & 0x3F]);
    }

    while (result.size() % 4) {
      result.push_back('=');
    }

    return result;
  }

  RenderSystem::RenderSystem() {};

  void RenderSystem::start() {
    Engine::get()->renderer_api->init();

    auto entity_manager = EntityManager::get();
    auto resource_manager = ResourceManager::get();

    resource_manager->load_shader(Shader::create("shaders::post_processing",
      "fragment/postprocessing.glsl",
      "vertex/postprocessing.glsl"));

    entity_manager->add_entity<PostProcessing>("Post Processing",
      "shaders::post_processing");

    entity_manager->for_each<Object>([](Object* object) { object->start(); });
    entity_manager->for_each<PostProcessing>(
      [](PostProcessing* post_processing) { post_processing->start(); });


    // if (fd == -1) {
    //   std::cout << "Opening fd" << std::endl;
    //   fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);

    //   if (fd == -1) {
    //     std::cout << "Failed to open shared memory" << std::endl;
    //     return;
    //   }

    //   if (ftruncate(fd, WIDTH * HEIGHT * PIXEL_SIZE) == -1) {

    //     std::cerr << "Error setting size of shared memory" << std::endl;
    //     return;
    //   }

    //   ptr = mmap(nullptr, WIDTH * HEIGHT * PIXEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //   if (ptr == MAP_FAILED) {
    //     std::cout << "Failed to map shared memory" << std::endl;
    //     return;
    //   }
    // }

    // if (ptr == nullptr || ptr == MAP_FAILED) {
    //   return;
    // }
  }

  void RenderSystem::fixed_update(double fixed_dt) {

  };

  void RenderSystem::pre_update(double dt) {
    auto engine = Engine::get();

    engine->framebuffer->bind();

    engine->renderer_api->enable_buffer_testing();
    engine->renderer_api->clear_color();
    engine->renderer_api->clear_buffers();
    engine->framebuffer->clear_attachment(1, -1);

    EntityManager::get()->for_each<Object>(
      [&](Object* object) { object->pre_update(); });
  };

  void RenderSystem::update(double dt) {
    auto entity_manager = EntityManager::get();

    entity_manager->for_each<Object>([&](Object* object) { object->update(); });
  };

  void RenderSystem::post_update(double dt) {
    std::vector<uint8_t> framebuffer(WIDTH * HEIGHT * PIXEL_SIZE);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer.data());

    auto app = WebsocketServer::get()->get_app();
    app->publish("broadcast", std::string_view(reinterpret_cast<const char*>(framebuffer.data()), framebuffer.size()), uWS::OpCode::BINARY, false);



    Engine::get()->framebuffer->unbind();

    auto entity_manager = EntityManager::get();

    entity_manager->for_each<Object>(
      [&](Object* object) { object->post_update(); });

    entity_manager->for_each<PostProcessing>(
      [&](PostProcessing* post_processing) { post_processing->post_update(); });
  };

  RenderSystem::~RenderSystem() {}

} // namespace astralix
