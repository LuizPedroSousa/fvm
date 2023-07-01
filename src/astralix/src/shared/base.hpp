#pragma once

#include "memory"

namespace astralix {

#define ASTRA_BIND_EVENT_FN(fn)                                                \
  [this](auto &&...args) -> decltype(auto) {                                   \
    return this->fn(std::forward<decltype(args)>(args)...);                    \
  }

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> create_scope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
};

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> create_ref(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
};

}; // namespace astralix