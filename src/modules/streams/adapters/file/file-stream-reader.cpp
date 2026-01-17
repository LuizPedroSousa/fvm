#include "file-stream-reader.hpp"
#include "assert.hpp"
#include "stream-buffer.hpp"

namespace astralix {

FileStreamReader::FileStreamReader(const std::filesystem::path &path)
    : StreamReader(), m_path(path) {
  m_file.open(m_path, std::ios::binary);
  m_file.seekg(0, std::ios::end);
  m_total_size = m_file.tellg();

  m_file.seekg(0, std::ios::beg);

  m_buffer = create_scope<StreamBuffer>(m_total_size);
}

FileStreamReader::~FileStreamReader() {}

void FileStreamReader::read() {
  ASTRA_EXCEPTION(!m_file.is_open(), "Cannot open file" + std::string(m_path))

  m_file.read(m_buffer->data(), m_buffer->size());

  auto total_read = m_file.gcount();

  ASTRA_EXCEPTION(total_read != m_total_size,
                  "Cannot read file" + std::string(m_path));

  if (m_file.eof()) {
    m_file.close();
  }
};

} // namespace astralix
//
