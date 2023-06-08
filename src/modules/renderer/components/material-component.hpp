#include "assimp/scene.h"
#include "components/base/component.hpp"
#include "resources/material.hpp"
#include "resources/texture.hpp"

class MaterialComponent : public Component<MaterialComponent> {
  public:
  MaterialComponent(COMPONENT_INIT_PARAMS);

  void update();
  void reset_material();

  void attach_material(ResourceID material);
  void attach_materials(std::vector<ResourceID> materials);
};