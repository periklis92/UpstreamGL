#pragma once

#include <cassert>
#include <iostream>
#include <string>

#define __TEST_FAIL(msg)                                                                           \
  std::string s = (std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) +           \
                   std::string(" in ") + std::string(__FUNCTION__));                               \
  std::cout << "ASSERTION FAILED:\n" << s << "\n" << msg << std::endl;                             \
  exit(EXIT_FAILURE);

#define TEST_ASSERT_EQUAL(x, y, msg)                                                               \
  {                                                                                                \
    if ((x) != (y)) {                                                                              \
      __TEST_FAIL(msg)                                                                             \
    }                                                                                              \
  }

#define TEST_ASSERT_GREATER(x, y, msg)                                                             \
  {                                                                                                \
    if ((x) <= (y)) {                                                                              \
      __TEST_FAIL(msg)                                                                             \
    }                                                                                              \
  }

#define TEST_ASSERT_GREATER_EQ(x, y, msg)                                                          \
  {                                                                                                \
    if ((x) < (y)) {                                                                               \
      __TEST_FAIL(msg)                                                                             \
    }                                                                                              \
  }

#define TEST_ASSERT_LESS(x, y, msg)                                                                \
  {                                                                                                \
    if ((x) >= (y)) {                                                                              \
      __TEST_FAIL(msg)                                                                             \
    }                                                                                              \
  }

#define TEST_ASSERT_LESS_EQ(x, y, msg)                                                             \
  {                                                                                                \
    if ((x) > (y)) {                                                                               \
      __TEST_FAIL(msg)                                                                             \
    }                                                                                              \
  }