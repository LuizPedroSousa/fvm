#pragma once
#include "imgui.h"

#include "imgui_internal.h"
#include "imgui_stdlib.h"

namespace astralix {
  class Layer {

  public:
    Layer(const std::string& name) : name(name) {}

    std::string name;

    virtual void start() = 0;
    virtual void update() = 0;
  };
} // namespace astralix
