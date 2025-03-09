#pragma once

#include "base.hpp"
#include "exceptions/base-exception.hpp"
#include <algorithm>
#include <vector>

namespace astralix {

static std::string to_lower(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

static size_t levenshtein_distance(const std::string &s1,
                                   const std::string &s2) {
  const size_t len1 = s1.size(), len2 = s2.size();
  std::vector<std::vector<size_t>> d(len1 + 1, std::vector<size_t>(len2 + 1));

  for (size_t i = 0; i <= len1; ++i)
    d[i][0] = i;
  for (size_t j = 0; j <= len2; ++j)
    d[0][j] = j;

  for (size_t i = 1; i <= len1; ++i) {
    for (size_t j = 1; j <= len2; ++j) {
      d[i][j] = std::min({d[i - 1][j] + 1, d[i][j - 1] + 1,
                          d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)});
    }
  }
  return d[len1][len2];
}

#define ASTRA_NOT_FOUND_EXCEPTION(EXPRESSION, table, id, type)                 \
  if (EXPRESSION) {                                                            \
    do {                                                                       \
      std::vector<std::string> suggestions;                                    \
      std::string id_lower = to_lower(id);                                     \
      for (const auto &[key, _] : table) {                                     \
        std::string key_lower = to_lower(key);                                 \
        if (key_lower.find(id_lower) != std::string::npos ||                   \
            levenshtein_distance(key_lower, id_lower) < 3) {                   \
          suggestions.push_back(key);                                          \
        }                                                                      \
      }                                                                        \
      std::string suggestion_msg =                                             \
          suggestions.empty()                                                  \
              ? ""                                                             \
              : (" Maybe did you mean one of these? \n" + std::string(BOLD) +  \
                 std::string(" - ") +                                          \
                 std::accumulate(                                              \
                     suggestions.begin(), suggestions.end(), std::string{},    \
                     [](const std::string &a, const std::string &b) {          \
                       return a.empty() ? b : a + "\n - " + b;                 \
                     }));                                                      \
                                                                               \
      ASTRA_EXCEPTION(true, type " with ID " BOLD, "[", id, "]", RESET,        \
                      " not found in ", BOLD, "ResourceManager", RESET,        \
                      suggestion_msg);                                         \
                                                                               \
    } while (0);                                                               \
  }

#define ASTRA_ASSERT(EXPRESSION, MESSAGE)                                      \
  if (EXPRESSION)                                                              \
  return BaseException(__FILE__, __LINE__, MESSAGE)

#define ASTRA_EXCEPTION(EXPRESSION, ...)                                       \
  if (EXPRESSION) {                                                            \
    throw BaseException(__FILE__, __FUNCTION__, __LINE__,                      \
                        build_exception_message(__VA_ARGS__));                 \
  }

#define ASTRA_ASSERT_EITHER(EXPRESSION)                                        \
  if (EXPRESSION.isLeft())                                                     \
  return EXPRESSION.left()

#define ASTRA_ASSERT_EITHER_THROW(EXPRESSION)                                  \
  if (EXPRESSION.isLeft())                                                     \
  throw EXPRESSION.left()

} // namespace astralix
