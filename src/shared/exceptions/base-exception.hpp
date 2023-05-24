#pragma once
#include "exception"
#include "string"

class BaseException : public std::exception {
  public:
  BaseException(const char *file, int line, const char *message);
  BaseException();

  const char *what() const noexcept override {
    auto parsed = std::string(m_file) + "\n" + std::to_string(m_line) + "\n" + std::string(m_message);

    char *result = new char[parsed.size() + 1]; // +1 for the terminating null
    std::copy(parsed.begin(), parsed.end(), result);

    result[parsed.size()] = '\0'; // add the terminating null

    return result;
  }

  private:
  const char *m_message;
  int m_line;
  const char *m_file;
};