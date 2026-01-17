#pragma once

#include "stream-reader.hpp"

namespace astralix {

class MemoryStreamReader : public StreamReader {
  virtual void read(void *data, size_t size) = 0;
};

} // namespace astralix
