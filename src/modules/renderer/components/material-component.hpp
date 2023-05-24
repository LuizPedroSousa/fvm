#include "components/base/component.hpp"
#include "resources/texture.hpp"

struct Material {
  Texture diffuse;
  Texture specular;
  float shinesses;
};

class MaterialComponent : public Component<MaterialComponent> {
  public:
  MaterialComponent(COMPONENT_INIT_PARAMS);

  Either<BaseException, Unit> load_maps(Either<BaseException, Texture> diffuse, Either<BaseException, Texture> specular, float shininess);

  private:
  Material m_material;
};