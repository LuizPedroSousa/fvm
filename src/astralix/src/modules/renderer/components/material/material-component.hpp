#pragma once
#include "components/component.hpp"
#include "string"

namespace astralix {

  class MaterialComponent : public Component<MaterialComponent> {
  public:
    MaterialComponent(COMPONENT_INIT_PARAMS);

    void update();
    void reset_material();

    void attach_material(ResourceID material);
    void attach_materials(std::vector<ResourceID> materials);

  private:
    static std::string get_name(const char* prefix, int count);
  };

} // namespace astralix
