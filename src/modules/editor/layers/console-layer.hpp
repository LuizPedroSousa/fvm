#include "entities/layer.hpp"

namespace astralix {

class ConsoleLayer : public Layer {
public:
  ConsoleLayer() : Layer("Console") {}

  void start() override;
  void update() override;
};

} // namespace astralix
