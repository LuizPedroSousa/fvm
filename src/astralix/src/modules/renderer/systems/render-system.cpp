#include "render-system.hpp"
#include "glad/glad.h"

#include "engine.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "iostream"
#include "scene-system.hpp"
#include "window.hpp"
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <iostream>
#include "vector"
#include <sys/mman.h>

typedef websocketpp::server<websocketpp::config::asio> server;
server ws_server;

#define SHM_NAME "/framebuffer_shm"
#define WIDTH 800
#define HEIGHT 600
#define PIXEL_SIZE 4  // RGBA




websocketpp::connection_hdl hdl;  // Global or member variable for the connection handle


// Map the shared memory into the process's address space
void* ptr;

namespace astralix {
  std::string base64_encode(const std::vector<unsigned char>& data) {
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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

    ws_server.init_asio();
    ws_server.set_reuse_addr(true);

    // Open a shared memory segment
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
      std::cerr << "Error opening shared memory" << std::endl;
      return;
    }

    // Set the size of the shared memory segment
    if (ftruncate(shm_fd, WIDTH * HEIGHT * PIXEL_SIZE) == -1) {
      std::cerr << "Error setting size of shared memory" << std::endl;
      return;
    }

    ptr = mmap(nullptr, WIDTH * HEIGHT * PIXEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
      std::cerr << "Error mapping shared memory" << std::endl;
      return;
    }



    // Open handler to capture the connection handle when a client connects
    ws_server.set_open_handler([&](websocketpp::connection_hdl _hdl) {
      hdl = _hdl;
      std::cout << "Client connected!" << std::endl;
      });


    ws_server.set_close_handler([&](websocketpp::connection_hdl _hdl) {
      std::cout << "Client disconnected!" << std::endl;
      hdl.reset();  // Limpa a referência para o WebSocket handle
      });

    std::thread([&]() {
      ws_server.listen(9002);
      ws_server.start_accept();
      ws_server.run();
      }).detach();

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
    size_t framebufferSize = 800 * 600 * 4; // Tamanho do framebuffer

    // Criar SharedArrayBuffer
    uint8_t* framebuffer = static_cast<uint8_t*>(ptr);

    // Read the OpenGL framebuffer into the vector
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);

    try {
      //ws_server.send(hdl, framebuffer, WIDTH * HEIGHT * 4, websocketpp::frame::opcode::binary);
    }
    catch (const websocketpp::exception& e) {
      std::cerr << "Error sending framebuffer data: " << e.what() << std::endl;
    }

    Engine::get()->framebuffer->unbind();

    auto entity_manager = EntityManager::get();

    entity_manager->for_each<Object>(
      [&](Object* object) { object->post_update(); });

    entity_manager->for_each<PostProcessing>(
      [&](PostProcessing* post_processing) { post_processing->post_update(); });
  };

  RenderSystem::~RenderSystem() {}

} // namespace astralix