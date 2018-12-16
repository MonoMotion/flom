#ifndef FLOM_TEST_OPERATORS_HPP
#define FLOM_TEST_OPERATORS_HPP

#include <flom/frame.hpp>
#include <flom/effector.hpp>

// Non-strict == operators to use in tests.

namespace flom {

bool operator==(const Frame& v1, const Frame& v2) {
  return almost_equal(v1, v2);
}

bool operator==(const Effector& v1, const Effector& v2) {
  return almost_equal(v1, v2);
}

bool operator==(const Location& v1, const Location& v2) {
  return almost_equal(v1, v2);
}

bool operator==(const Rotation& v1, const Rotation& v2) {
  return almost_equal(v1, v2);
}

}

#endif
