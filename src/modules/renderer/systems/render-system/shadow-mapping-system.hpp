#pragma once
#include "systems/system.hpp"
#include "entities/object.hpp"
#include "framebuffer.hpp"
namespace astralix {

  class ShadowMappingSystem : public System<ShadowMappingSystem> {
  public:
    ShadowMappingSystem();
    ~ShadowMappingSystem();

    void start() override;
    void fixed_update(double fixed_dt) override;
    void pre_update(double dt) override;
    void update(double dt) override;

    void bind_depth(Object* object);

    Ref<Framebuffer> m_framebuffer;
  };

} // namespace astralix
