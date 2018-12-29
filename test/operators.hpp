//
// Copyright 2018 coord.e
//
// This file is part of Flom.
//
// Flom is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Flom is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Flom.  If not, see <http://www.gnu.org/licenses/>.
//

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
