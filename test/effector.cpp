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
#include <flom/interpolation.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(effector)

RC_BOOST_PROP(diff_mul_scalar,
              (const flom::EffectorDifference &d, unsigned short v)) {
  RC_PRE(v != 0);

  auto const d1 = d * v;
  auto d2 = d;
  for (std::size_t i = 0; i < v - 1; i++) {
    d2 += d;
  }
  RC_ASSERT(d1 == d2);
}

RC_BOOST_PROP(diff_add, (const flom::Effector &e1, unsigned short mul)) {
  auto e2 = e1.new_compatible_effector();
  auto const diff1 = e1 - e2;
  auto const diff2 = diff1 * mul;
  RC_ASSERT((e1 + diff1) + diff2 == e1 + (diff1 + diff2));
}

RC_BOOST_PROP(sub, (const flom::Effector &e1)) {
  auto e2 = e1.new_compatible_effector();
  auto const diff = e1 - e2;
  RC_ASSERT(e2 + diff == e1);
}

RC_BOOST_PROP(interpolation_location,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.location && e2.location);

  const double t = static_cast<double>(*rc::gen::inRange(0, 100)) / 100;

  auto e3 = flom::interpolate(t, e1, e2);
  auto l = flom::interpolate(t, *e1.location, *e2.location);
  RC_ASSERT(e3.location->vec == l.vec);
}

RC_BOOST_PROP(interpolation_rotation,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.rotation && e2.rotation);

  const double t = static_cast<double>(*rc::gen::inRange(0, 100)) / 100;

  auto e3 = flom::interpolate(t, e1, e2);
  auto r = flom::interpolate(t, *e1.rotation, *e2.rotation);
  RC_ASSERT(e3.rotation->quat == r.quat);
}

BOOST_AUTO_TEST_SUITE_END()
