#pragma once
#include "strategy.hpp"
#include <cstddef>

namespace astralix {

class PointStrategy : public LightStrategy {
public:
  struct Exposure {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };

  struct Attenuation {
    float constant = 1.0f;
    float linear = 0.045f;
    float quadratic = 0.0075f;
  };

  struct Point {
    Attenuation attenuation;
    Exposure exposure = {.ambient = glm::vec3(5.0f),
                         .diffuse = glm::vec3(2.0f),
                         .specular = glm::vec3(1.0f)};
  };

  PointStrategy(Point point) : m_point(point) {};

  void update(IEntity *source, Object *object, EntityID &camera_id,
              size_t &index) override;

  Point m_point;
};

} // namespace astralix
