#include "post-processing.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/render/render-component.hpp"
#include "components/resource/resource-component.hpp"

namespace astralix {

PostProcessing::PostProcessing(ENTITY_INIT_PARAMS) : ENTITY_INIT() {
  add_component<RenderComponent>();
  add_component<MeshComponent>()->attach_mesh(Mesh::quad(1.0f));
  add_component<ResourceComponent>();
}

} // namespace astralix
