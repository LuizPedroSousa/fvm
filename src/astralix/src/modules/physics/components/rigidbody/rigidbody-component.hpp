#include "ecs/components/component.hpp"

namespace astralix {
class RigidBodyComponent : public Component<RigidBodyComponent> {
public:
  RigidBodyComponent(COMPONENT_INIT_PARAMS, float gravity = 0.5f,
                     float velocity = 2.0f, float acceleration = 2.0f,
                     float drag = 0.0f, float mass = 1.0f);

  void update(float dt);

private:
  float velocity;
  float gravity;
  float drag;
  float mass;
  float acceleration;
};

} // namespace astralix
