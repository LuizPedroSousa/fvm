#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"
#include "./shader-renderer.hpp"
#include <vector>
#include "./object.hpp"

class ObjectRenderer
{
public:
  std::vector<Object> objects;
  ObjectRenderer(){};
  void spawn(Object obj);
  void spawnMany(Object *obj, size_t size);

  void start_all();
  void render_all();
};