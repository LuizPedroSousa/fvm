#pragma once
#include "exceptions/base-exception.hpp"
#include "iostream"
#include "stdio.h"
#include "tuple"
#include "vector"
#include <stdexcept>

#include <type_traits>

#define ASSERT(EXPRESSION, MESSAGE)                                            \
    if (EXPRESSION)                                                            \
    return BaseException(__FILE__, __LINE__, MESSAGE)

#define ASSERT_THROW(EXPRESSION, MESSAGE)                                      \
    if (EXPRESSION) {                                                          \
        throw BaseException(__FILE__, __LINE__, MESSAGE);                      \
    }

#define ASSERT_COMPARE(EXPRESSION)                                             \
    if (EXPRESSION.isLeft())                                                   \
    return EXPRESSION.left()

#define ASSERT_COMPARE_THROW(EXPRESSION)                                       \
    if (EXPRESSION.isLeft())                                                   \
    throw EXPRESSION.left()

class Unit {
  public:
    Unit() {}
};

template <class L, class R> class Either {
  private:
    L left_value;
    R right_value;

    bool has_left;

  public:
    Either(L exception) : left_value(exception), has_left(true) {}

    Either(L &exception) : left_value(exception), has_left(true) {}

    Either(R result) : right_value(result), has_left(false) {}

    bool isRight() const { return !has_left; }

    bool isLeft() const { return has_left; }

    L left() const {
        if (isLeft()) {
            return left_value;
        }
        throw std::logic_error("Either does not contain a left value.");
    }

    R right() const {
        if (isRight()) {
            return right_value;
        }
        throw std::logic_error("Either does not contain a right value.");
    }

    template <typename F>
    auto MapLeft(F f) -> Either<decltype(f(left_value)), R> {
        if (isLeft()) {
            return Either<decltype(f(left_value)), R>(f(right_value));
        } else {
            return Either<decltype(f(left_value)), R>(right_value);
        }
    }

    template <typename F>
    auto MapRight(F f) -> Either<L, decltype(f(right_value))> {
        if (isRight()) {
            return Either<L, decltype(f(right_value))>(left_value);
        } else {
            return Either<L, decltype(f(right_value))>(f(right_value));
        }
    }
};
