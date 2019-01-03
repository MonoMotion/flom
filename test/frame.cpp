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

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(frame)

RC_BOOST_PROP(joint_list, (const flom::Frame &f)) {
  std::unordered_set<std::string> o1, o2;
  std::transform(std::cbegin(f.positions), std::cend(f.positions),
                 std::inserter(o1, std::end(o1)),
                 [](auto const &p) { return p.first; });
  boost::copy(f.joint_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(effector_list, (const flom::Frame &f)) {
  std::unordered_set<std::string> o1, o2;
  std::transform(std::cbegin(f.effectors), std::cend(f.effectors),
                 std::inserter(o1, std::end(o1)),
                 [](auto const &p) { return p.first; });
  boost::copy(f.effector_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(interpolation, (const flom::Frame &f1)) {
  const double t = static_cast<double>(*rc::gen::inRange(0, 100)) / 100;
  const double d = static_cast<double>(*rc::gen::inRange(0, 100)) / 100;

  auto f2 = f1 * d;
  auto const f3 = flom::interpolate(t, f1, f2);
  for (auto &&[key, val] : f3.positions) {
    RC_ASSERT(val ==
              flom::interpolate(t, f1.positions.at(key), f2.positions.at(key)));
  }
  for (auto &&[key, val] : f3.effectors) {
    RC_ASSERT(val ==
              flom::interpolate(t, f1.effectors.at(key), f2.effectors.at(key)));
  }
}

BOOST_AUTO_TEST_SUITE_END()
