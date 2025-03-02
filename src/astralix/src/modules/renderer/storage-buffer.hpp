#pragma once

#include "base.hpp"
namespace astralix {

class StorageBuffer {
public:
  virtual ~StorageBuffer() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void bind_base(uint32_t point = 0) const = 0;
  virtual void set_data(const void *data, uint32_t size) const = 0;

  static Ref<StorageBuffer> create(uint32_t size);
};

} // namespace astralix
