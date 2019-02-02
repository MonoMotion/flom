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

#include <boost/range/algorithm.hpp>

#include <unordered_set>

#include <flom/frame.hpp>
#include <flom/interpolation.hpp>

#include "comparison.hpp"
#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(frame)

RC_BOOST_PROP(diff_mul_scalar,
              (const flom::FrameDifference &d, unsigned short v)) {
  RC_PRE(v != 0);

  auto const d1 = d * v;
  auto d2 = d;
  for (std::size_t i = 0; i < v - 1; i++) {
    d2 += d;
  }
  FLOM_ALMOST_EQUAL(d1, d2);
}

RC_BOOST_PROP(diff_add, (const flom::Frame &f1, unsigned short mul)) {
  auto f2 = f1.new_compatible_frame();
  auto const diff1 = f1 - f2;
  auto const diff2 = diff1 * mul;
  FLOM_ALMOST_EQUAL((f1 + diff1) + diff2, f1 + (diff1 + diff2));
}

RC_BOOST_PROP(sub, (const flom::Frame &f1)) {
  auto f2 = f1.new_compatible_frame();
  auto const diff = f1 - f2;
  FLOM_ALMOST_EQUAL(f2 + diff, f1);
}

RC_BOOST_PROP(new_compatible_frame, (const flom::Frame &f1)) {
  auto const f2 = f1.new_compatible_frame();
  RC_ASSERT(f1.is_compatible(f2));
  RC_ASSERT(f2.is_compatible(f1));
}

RC_BOOST_PROP(joint_list, (const flom::Frame &f)) {
  std::unordered_set<std::string> o1, o2;
  std::transform(std::cbegin(f.positions()), std::cend(f.positions()),
                 std::inserter(o1, std::end(o1)),
                 [](auto const &p) { return p.first; });
  boost::copy(f.joint_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(effector_list, (const flom::Frame &f)) {
  std::unordered_set<std::string> o1, o2;
  std::transform(std::cbegin(f.effectors()), std::cend(f.effectors()),
                 std::inserter(o1, std::end(o1)),
                 [](auto const &p) { return p.first; });
  boost::copy(f.effector_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(interpolation, (const flom::Frame &f1)) {
  const double t = static_cast<double>(*rc::gen::inRange(0, 100)) / 100;

  auto const f2 = f1.new_compatible_frame();
  auto const f3 = flom::interpolate(t, f1, f2);
  for (auto &&[key, val] : f3.positions()) {
    auto const interp =
        flom::interpolate(t, f1.positions().at(key), f2.positions().at(key));
    FLOM_ALMOST_EQUAL(val, interp);
  }
  for (auto &&[key, val] : f3.effectors()) {
    auto const interp =
        flom::interpolate(t, f1.effectors().at(key), f2.effectors().at(key));
    FLOM_ALMOST_EQUAL(val, interp);
  }
}

BOOST_AUTO_TEST_SUITE_END()
