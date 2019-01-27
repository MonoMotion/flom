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

#include <flom/motion.hpp>

#include "comparison.hpp"
#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_frame_range)

RC_BOOST_PROP(frames_range_t, (flom::Motion m)) {
  auto const fps = *rc::gen::positive<double>();

  RC_PRE(m.length() >= fps);
  m.set_loop(flom::LoopType::None);

  unsigned long count = 0;
  for (auto const &[t, frame] : m.frames(fps)) {
    RC_ASSERT(t == count * fps);
    if (t > m.length()) {
      RC_FAIL("Iteration does not over");
      break;
    }
    count++;
  }
}

RC_BOOST_PROP(frames_range_none, (const flom::Motion &m)) {
  auto const fps = *rc::gen::positive<double>();

  RC_PRE(m.length() >= fps);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_ASSERT(m.is_valid());

  unsigned long count = 0;
  for (auto const &[t, frame] : m.frames(fps)) {
    RC_ASSERT(frame == m.frame_at(t));
    if (t > m.length()) {
      RC_FAIL("Iteration does not over");
      break;
    }
    count++;
  }
}

RC_BOOST_PROP(frames_range_wrap, (const flom::Motion &m)) {
  auto const fps = *rc::gen::positive<double>();

  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_ASSERT(m.is_valid());

  unsigned long count = 0;
  for (auto const &[t, frame] : m.frames(fps)) {
    FLOM_ALMOST_EQUAL(frame, m.frame_at(t));
    if (t > m.length() * 2) {
      break;
    }
    count++;
  }
}

BOOST_AUTO_TEST_SUITE_END()
