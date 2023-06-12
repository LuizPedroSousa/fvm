#include "exceptions/base-exception.hpp"

namespace astralix {

BaseException::BaseException() {}

BaseException::BaseException(const char *file, int line, std::string message)
    : m_line(line), m_file(file), m_message(message) {}

} // namespace astralix