#include "components/base/component.hpp"
#include "entities/object.hpp"
#include "utils/guid.hpp"
#include "vector"

class RenderComponent : public Component<RenderComponent> {
  public:
  RenderComponent(COMPONENT_INIT_PARAMS);

  RenderComponent() {}

  void start();
  void update();

  private:
  void draw(Object *object);
  void create_buffers(Object *object);
};