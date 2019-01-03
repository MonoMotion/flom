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
