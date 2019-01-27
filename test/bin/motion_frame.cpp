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

#include "comparison.hpp"
#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_frame)

RC_BOOST_PROP(retrieve_frame, (flom::Motion m)) {
  auto t = *rc::gen::nonNegative<double>();

  RC_PRE(m.length() != 0);
  // RC_PRE(m.length() >= t);
  while (m.length() < t) {
    t -= m.length();
  }

  auto const frame = m.frame_at(t);

  flom::Frame expected_frame;
  {
    auto range = m.keyframes();
    auto const it = std::find_if(range.begin(), range.end(),
                                 [t](auto const &p) { return p.first == t; });
    if (it == range.end()) {
      struct Comp {
        using value_type = decltype(it)::value_type;
        bool operator()(const value_type &p, double t) const {
          return p.first < t;
        }
        bool operator()(double t, const value_type &p) const {
          return t < p.first;
        }
      };
      auto [l, u] = std::equal_range(range.begin(), range.end(), t, Comp{});
      // TODO: Use -> (after #43)
      auto const t1 = (*std::next(l, -1)).first;
      auto const t2 = (*u).first;
      auto const &f1 = (*std::next(l, -1)).second;
      auto const &f2 = (*u).second;
      expected_frame = flom::interpolate((t - t1) / (t2 - t1), f1, f2);
    } else {
      expected_frame = (*it).second;
    }
  }

  FLOM_ALMOST_EQUAL(frame, expected_frame);
}

RC_BOOST_PROP(retrieve_frame_zero, (flom::Motion m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  {
    // Clear keyframes
    std::vector<double> t_list;
    for (auto const &[t, f] : m.keyframes()) {
      if (t != 0) {
        t_list.push_back(t);
      }
    }
    for (auto time : t_list) {
      m.delete_keyframe(time);
    }
    RC_ASSERT(m.length() == 0);
  }

  auto const frame = m.frame_at(t);

  auto const expected_frame = m.frame_at(0);

  FLOM_ALMOST_EQUAL(frame, expected_frame);
}

RC_BOOST_PROP(retrieve_frame_over, (const flom::Motion &m)) {
  auto t = *rc::gen::nonNegative<double>();

  auto const len = m.length();

  RC_PRE(len != 0);

  // RC_PRE(len < t);
  if (len >= t) {
    t += len;
  }

  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  auto const frame = m.frame_at(t);

  auto const mul = static_cast<unsigned>(t / len);
  auto const last = m.frame_at(len);
  auto const init = m.frame_at(0);
  auto const expected_frame =
      m.frame_at(std::fmod(t, len)) + (last - init) * mul;

  FLOM_ALMOST_EQUAL(frame, expected_frame);
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
