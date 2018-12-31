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

RC_BOOST_PROP(retrieve_frame_wrap, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  auto frame = m.frame_at(t);

  flom::Frame expected_frame;
  if (m.length() == 0) {
    expected_frame = m.frame_at(0);
  } else {
    auto len = m.length();
    auto mul = static_cast<unsigned>(t / len);
    expected_frame = m.frame_at(std::fmod(t, len)) + m.frame_at(len) * mul;
  }

  // Using non-strict version of operator== defined in operators.hpp
  RC_ASSERT(frame == expected_frame);
}

RC_BOOST_PROP(retrieve_frame_none, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.length() >= t);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_ASSERT(m.is_valid());

  auto frame = m.frame_at(t);

  flom::Frame expected_frame;
  if (m.length() == 0) {
    expected_frame = m.frame_at(0);
  } else {
    auto len = m.length();
    auto mul = static_cast<unsigned>(t / len);
    expected_frame = m.frame_at(std::fmod(t, len)) + m.frame_at(len) * mul;
  }

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

RC_BOOST_PROP(frames_range_none, (const flom::Motion &m, double fps)) {
  RC_PRE(fps > 0);
  RC_PRE(m.length() >= fps);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_ASSERT(m.is_valid());

  unsigned long count = 0;
  for (auto const &frame : m.frames(fps)) {
    RC_ASSERT(frame == m.frame_at(count * fps));
    if (count * fps > m.length()) {
      RC_FAIL("Iteration does not over");
      break;
    }
    count++;
  }
}

RC_BOOST_PROP(frames_range_wrap, (const flom::Motion &m, double fps)) {
  RC_PRE(fps > 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  unsigned long count = 0;
  for (auto const &frame : m.frames(fps)) {
    RC_ASSERT(frame == m.frame_at(count * fps));
    if (count * fps > m.length() * 2) {
      break;
    }
    count++;
  }
}

RC_BOOST_PROP(in_range_t_none, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_ASSERT(m.is_valid());

  RC_ASSERT((m.length() >= t) == m.is_in_range_at(t));
}

RC_BOOST_PROP(in_range_t_wrap, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  RC_ASSERT(m.is_in_range_at(t));
}

RC_BOOST_PROP(joint_list, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.is_in_range_at(t));
  RC_ASSERT(m.is_valid());

  std::unordered_set<std::string> o1, o2;
  boost::copy(m.frame_at(t).joint_names(), std::inserter(o1, std::end(o1)));
  boost::copy(m.joint_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(effector_list, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.is_in_range_at(t));
  RC_ASSERT(m.is_valid());

  std::unordered_set<std::string> o1, o2;
  boost::copy(m.frame_at(t).effector_names(), std::inserter(o1, std::end(o1)));
  boost::copy(m.effector_names(), std::inserter(o2, std::end(o2)));

  RC_ASSERT(o1 == o2);
}

RC_BOOST_PROP(new_frame, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());
  auto const frame = m.new_keyframe();
  RC_ASSERT(m.is_valid_frame(frame));
}

RC_BOOST_PROP(dump_load, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());

  auto const path = std::filesystem::temp_directory_path() / "out.fom";

  {
    std::ofstream f(path, std::ios::binary);

    // no throws
    m.dump(f);
  }

  {
    std::ifstream f(path, std::ios::binary);

    // no throws
    auto const m2 = m.load(f);
    std::filesystem::remove(path);

    RC_ASSERT(m == m2);
  }
}

RC_BOOST_PROP(dump_load_json, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());

  auto const path = std::filesystem::temp_directory_path() / "out.json";

  {
    std::ofstream f(path);

    // no throws
    m.dump_json(f);
  }

  {
    std::ifstream f(path);

    // no throws
    auto const m2 = m.load_json(f);
    std::filesystem::remove(path);

    RC_ASSERT(m == m2);
  }
}

BOOST_AUTO_TEST_SUITE_END()
