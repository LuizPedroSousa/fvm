#pragma once
#include "imgui/imgui.h"

#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace astralix {
class Layer {

public:
  Layer(const std::string &name) : name(name) {}

  std::string name;

  virtual void start() = 0;
  virtual void update() = 0;
};
} // namespace astralix
