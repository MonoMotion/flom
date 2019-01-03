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

#include <cmath>

#include <flom/errors.hpp>
#include <flom/motion.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_frame)

RC_BOOST_PROP(retrieve_frame_zero, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.length() == 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  auto const frame = m.frame_at(t);

  auto const expected_frame = m.frame_at(0);

  // Using non-strict version of operator== defined in operators.hpp
  RC_ASSERT(frame == expected_frame);
}

RC_BOOST_PROP(retrieve_frame_over, (const flom::Motion &m)) {
  auto const t = *rc::gen::nonNegative<double>();

  auto const len = m.length();

  RC_PRE(len != 0);
  RC_PRE(len < t);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  auto const frame = m.frame_at(t);

  auto const mul = static_cast<unsigned>(t / len);
  auto last = m.frame_at(len);
  auto const expected_frame = m.frame_at(std::fmod(t, len)).compose(last.repeat(mul));

  // Using non-strict version of operator== defined in operators.hpp
  RC_ASSERT(frame == expected_frame);
}

RC_BOOST_PROP(retrieve_frame_none_throw, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());
  RC_PRE(m.loop() == flom::LoopType::None);

  // Using unsigned long because RapidCheck doesn't support gen::inRange<double>
  RC_PRE(m.length() < std::numeric_limits<unsigned long>::max());
  const auto lower = static_cast<unsigned long>(m.length() + 1);
  const auto upper = std::numeric_limits<unsigned long>::max();
  double t = *rc::gen::inRange<unsigned long>(lower, upper);

  RC_ASSERT_THROWS_AS(m.frame_at(t), flom::errors::OutOfFramesError);
}

RC_BOOST_PROP(invalid_time, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());

  const double nan = std::numeric_limits<double>::quiet_NaN();
  RC_ASSERT_THROWS_AS(m.frame_at(-1), flom::errors::InvalidTimeError);
  RC_ASSERT_THROWS_AS(m.frame_at(nan), flom::errors::InvalidTimeError);
}

BOOST_AUTO_TEST_SUITE_END()
