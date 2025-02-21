#include "exceptions/base-exception.hpp"
#include "log.hpp"
#include <sstream>

namespace astralix {

BaseException::BaseException(const char *file, int line, std::string message)
    : m_line(line), m_file(file), m_message(message) {
  std::ostringstream oss;
  oss << BOLD << RED << "\n=== Astralix Exception ===" << RESET << "\n"
      << BOLD << CYAN << "File: " << RESET << m_file << "\n"
      << BOLD << CYAN << "Line: " << RESET << m_line << "\n"
      << BOLD << YELLOW << "Message: " << RESET << m_message << "\n"
      << BOLD << RED << "==========================" << RESET;
  m_formatted_message = oss.str();
}

} // namespace astralix
