#include "layers/managers/layer-manager.hpp"

namespace astralix {

  LayerManager* LayerManager::m_instance = nullptr;

  void LayerManager::init() {
    if (!m_instance)
      m_instance = new LayerManager;
  }

  LayerManager::LayerManager() { m_context = new LayerContext; };

  LayerManager::~LayerManager() { delete m_context; };

} // namespace astralix