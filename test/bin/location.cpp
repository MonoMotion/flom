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

BOOST_AUTO_TEST_SUITE(location)

RC_BOOST_PROP(mul_scalar, (const flom::Location &loc, unsigned short v)) {
  RC_PRE(v != 0);

  auto const loc1 = loc * v;
  auto loc2 = loc;
  for (std::size_t i = 0; i < v - 1; i++) {
    loc2 += loc;
  }
  FLOM_ALMOST_EQUAL(loc1, loc2);
}

RC_BOOST_PROP(sub, (const flom::Location &loc1, const flom::Location &loc2)) {
  auto const res = loc1 - loc2;
  auto const div_vec = loc1.vector() - loc2.vector();
  FLOM_ALMOST_EQUAL(res, flom::Location{div_vec});
}

RC_BOOST_PROP(add, (const flom::Location &loc1, const flom::Location &loc2)) {
  auto const res = loc1 + loc2;
  auto const add_vec = loc1.vector() + loc2.vector();
  FLOM_ALMOST_EQUAL(res, flom::Location{add_vec});
}

RC_BOOST_PROP(values, (const flom::Location &v)) {
  const auto [x, y, z] = v.xyz();
  RC_ASSERT(x == v.x());
  RC_ASSERT(y == v.y());
  RC_ASSERT(z == v.z());
}

RC_BOOST_PROP(set_values, (flom::Location v, double x, double y, double z)) {
  v.set_xyz(x, y, z);
  RC_ASSERT(x == v.x());
  RC_ASSERT(y == v.y());
  RC_ASSERT(z == v.z());
}

BOOST_AUTO_TEST_SUITE_END()
