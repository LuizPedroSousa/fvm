
#include "hdr-system.hpp"
#include "components/mesh/mesh-component.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "glad//glad.h"
#include "managers/entity-manager.hpp"
#include "managers/resource-manager.hpp"
#include <cmath>

namespace astralix {

HDRSystem::HDRSystem() {}

void HDRSystem::pre_update(double dt) {}

void HDRSystem::fixed_update(double fixed_dt) {}

void HDRSystem::start() {
  auto resource_manager = ResourceManager::get();

  auto shader = resource_manager->load_shader(Shader::create(
      "shaders::hdr", "fragment/hdr.glsl", "vertex/postprocessing.glsl"));

  EntityManager::get()->add_entity<PostProcessing>("HDR", "shaders::hdr");
}

void HDRSystem::update(double dt) {}

HDRSystem::~HDRSystem() {}

} // namespace astralix
