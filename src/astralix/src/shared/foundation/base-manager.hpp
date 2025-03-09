#pragma once
#include "base.hpp"

namespace astralix {

template <typename T> class BaseManager {
public:
  static Ref<T> get() { return m_instance; };

  static void init();

protected:
  static Ref<T> m_instance;
};

template <typename T> void BaseManager<T>::init() {
  m_instance = create_ref<T>();
}

template <typename T> Ref<T> BaseManager<T>::m_instance = nullptr;

} // namespace astralix
