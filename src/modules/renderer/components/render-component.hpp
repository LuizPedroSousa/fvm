#include "components/base/component.hpp"
#include "entities/object.hpp"
#include "functional"
#include "utils/guid.hpp"
#include "vector"

struct DrawCallback {
  std::function<void()> before;
  std::function<void()> after;
};

class RenderComponent : public Component<RenderComponent> {
  public:
  RenderComponent(COMPONENT_INIT_PARAMS);

  RenderComponent() {}

  void start(Object *owner);
  void update(Object *owner);
  void pre_update(Object *owner);

  void on_before_draw(std::function<void()> callback) {
    m_draw_callbacks.before = callback;
  };

  void on_after_draw(std::function<void()> callback) {
    m_draw_callbacks.after = callback;
  };

  private:
  DrawCallback m_draw_callbacks;
};