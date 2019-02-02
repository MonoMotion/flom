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

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include <flom/effector.hpp>

#include "comparison.hpp"
#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(rotation)

RC_BOOST_PROP(normalized, (const flom::Rotation &rot)) {
  auto const normalized = flom::Rotation{rot.quaternion().normalized()};
  FLOM_ALMOST_EQUAL(normalized, rot);
}

RC_BOOST_PROP(mul_scalar, (const flom::Rotation &rot, unsigned short v)) {
  RC_PRE(v != 0);

  auto const rot1 = rot * v;
  auto rot2 = rot;
  for (std::size_t i = 0; i < v - 1; i++) {
    rot2 += rot;
  }
  FLOM_ALMOST_EQUAL(rot1, rot2);
}

RC_BOOST_PROP(sub, (const flom::Rotation &rot1, const flom::Rotation &rot2)) {
  auto const res = rot1 - rot2;
  auto const div_quat = rot1.quaternion() * rot2.quaternion().conjugate();
  FLOM_ALMOST_EQUAL(res, flom::Rotation{div_quat});
}

RC_BOOST_PROP(add, (const flom::Rotation &rot1, const flom::Rotation &rot2)) {
  auto const res = rot1 + rot2;
  auto const mul_quat = rot1.quaternion() * rot2.quaternion();
  FLOM_ALMOST_EQUAL(res, flom::Rotation{mul_quat});
}

RC_BOOST_PROP(values, (const flom::Rotation &v)) {
  const auto [w, x, y, z] = v.wxyz();
  RC_ASSERT(w == v.w());
  RC_ASSERT(x == v.x());
  RC_ASSERT(y == v.y());
  RC_ASSERT(z == v.z());
}

RC_BOOST_PROP(set_values_normalized,
              (flom::Rotation v, double w, double x, double y, double z)) {
  RC_PRE(flom::Rotation::value_type(w, x, y, z).norm() != 0);

  v.set_wxyz(w, x, y, z);

  FLOM_ALMOST_EQUAL(v.quaternion().norm(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
