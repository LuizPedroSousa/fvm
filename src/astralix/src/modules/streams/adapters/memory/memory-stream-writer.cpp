#include "memory-stream-writer.hpp"

namespace astralix {

void MemoryStreamWriter::flush() { m_buffer->reset(); };

} // namespace astralix
