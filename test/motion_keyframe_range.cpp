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

#include <flom/errors.hpp>
#include <flom/motion.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_keyframe_range)

RC_BOOST_PROP(keyframe_range, (flom::Motion m)) {
  for (auto const &[t, keyframe] : m.keyframes()) {
    RC_ASSERT(m.is_valid_frame(keyframe));
    RC_ASSERT(m.frame_at(t) == keyframe);
  }
}

RC_BOOST_PROP(keyframe_range_assign, (flom::Motion m)) {
  auto const f = m.new_keyframe();
  for (auto &&[t, keyframe] : m.keyframes()) {
    keyframe = f;
  }
  for (auto const &[t, keyframe] : m.keyframes()) {
    RC_ASSERT(keyframe == f);
  }
}

RC_BOOST_PROP(keyframe_range_checked, (flom::Motion m, const flom::Frame &f)) {
  RC_PRE(!m.is_valid_frame(f));
  for (auto &&[t, keyframe] : m.keyframes()) {
    RC_ASSERT_THROWS_AS(keyframe = f, flom::errors::InvalidFrameError);
  }
}

BOOST_AUTO_TEST_SUITE_END()
