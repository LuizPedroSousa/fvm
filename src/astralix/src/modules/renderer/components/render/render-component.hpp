#include "ecs/components/component.hpp"
#include "ecs/guid.hpp"
#include "entities/object.hpp"
#include "functional"
#include "vector"

namespace astralix {

struct DrawCallback {
  std::function<void()> before;
  std::function<void()> after;
};

class RenderComponent : public Component<RenderComponent> {
public:
  RenderComponent(COMPONENT_INIT_PARAMS);

  RenderComponent() {}

  void start(IEntity *owner);
  void update(IEntity *owner);
  void pre_update(IEntity *owner);

  void on_before_draw(std::function<void()> callback) {
    m_draw_callbacks.before = callback;
  };

  void on_after_draw(std::function<void()> callback) {
    m_draw_callbacks.after = callback;
  };

private:
  DrawCallback m_draw_callbacks;
};

} // namespace astralix