#pragma once
#include "./object.hpp"
#include "./shader-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include <vector>

#include "renderer.hpp"

class ObjectRenderer : public Renderer {
  public:
  std::vector<Object> objects;
  ObjectRenderer(){};
  void spawn(Object obj);
  void spawnMany(Object *obj, size_t size);

  void edit_object();

  void start_all() override;
  void render_all() override;
};