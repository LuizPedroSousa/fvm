#pragma once

#include "base.hpp"
#include "exceptions/base-exception.hpp"

namespace astralix {

#define ASTRA_ASSERT(EXPRESSION, MESSAGE)                                      \
  if (EXPRESSION)                                                              \
  return BaseException(__FILE__, __LINE__, MESSAGE)

#define ASTRA_EXCEPTION(EXPRESSION, MESSAGE)                                   \
  if (EXPRESSION) {                                                            \
    throw BaseException(__FILE__, __LINE__, MESSAGE);                          \
  }

#define ASTRA_ASSERT_EITHER(EXPRESSION)                                        \
  if (EXPRESSION.isLeft())                                                     \
  return EXPRESSION.left()

#define ASTRA_ASSERT_EITHER_THROW(EXPRESSION)                                  \
  if (EXPRESSION.isLeft())                                                     \
  throw EXPRESSION.left()

} // namespace astralix
