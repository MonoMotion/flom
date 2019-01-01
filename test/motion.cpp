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
#include <boost/range/size.hpp>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <unordered_set>

#include <flom/errors.hpp>
#include <flom/motion.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion)

BOOST_AUTO_TEST_CASE(empty_motion_valid) {
  std::unordered_set<std::string> j, e;
  auto const empty = flom::Motion(j, e);
  BOOST_TEST(empty.is_valid());
}

RC_BOOST_PROP(empty_named_motion_valid,
              (const std::string &name,
               const std::unordered_set<std::string> &j,
               const std::unordered_set<std::string> &e)) {
  auto const empty = flom::Motion(j, e, name);
  RC_ASSERT(empty.is_valid());
}

RC_BOOST_PROP(in_range_t_none, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.loop() == flom::LoopType::None);
  RC_ASSERT(m.is_valid());

  RC_ASSERT((m.length() >= t) == m.is_in_range_at(t));
}

RC_BOOST_PROP(in_range_t_wrap, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  RC_ASSERT(m.is_in_range_at(t));
}

RC_BOOST_PROP(joint_list, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.is_in_range_at(t));
  RC_ASSERT(m.is_valid());

  std::unordered_set<std::string> o1, o2;
  boost::copy(m.frame_at(t).joint_names(), std::inserter(o1, std::end(o1)));
  boost::copy(m.joint_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(effector_list, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.is_in_range_at(t));
  RC_ASSERT(m.is_valid());

  std::unordered_set<std::string> o1, o2;
  boost::copy(m.frame_at(t).effector_names(), std::inserter(o1, std::end(o1)));
  boost::copy(m.effector_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

BOOST_AUTO_TEST_SUITE_END()
