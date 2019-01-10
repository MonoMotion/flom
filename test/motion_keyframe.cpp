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

#include <boost/range/size.hpp>

#include <flom/errors.hpp>
#include <flom/motion.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_keyframe)

RC_BOOST_PROP(new_keyframe, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());
  auto const frame = m.new_keyframe();
  RC_ASSERT(m.is_valid_frame(frame));
}

RC_BOOST_PROP(insert_keyframe, (flom::Motion m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_ASSERT(m.is_valid());
  auto const frame = m.new_keyframe();
  m.insert_keyframe(t, frame);

  RC_ASSERT(m.frame_at(t) == frame);
}

RC_BOOST_PROP(insert_init_keyframe, (flom::Motion m)) {
  //
  // Check if insertion to t == 0 is working properly
  // Issue: #34
  //

  RC_PRE(!boost::empty(m.joint_names()));
  RC_PRE(!boost::empty(m.effector_names()));

  auto frame = m.new_keyframe();
  // Modify frame in some way
  frame.set_position(frame.positions().begin()->first, 1);

  m.insert_keyframe(0, frame);
  RC_ASSERT(m.frame_at(0) == frame);
}

RC_BOOST_PROP(insert_keyframe_invalid, (flom::Motion m, const flom::Frame &f)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(!m.is_valid_frame(f));
  RC_ASSERT_THROWS_AS(m.insert_keyframe(t, f), flom::errors::InvalidFrameError);
}

RC_BOOST_PROP(insert_keyframe_incompatible_effector, (flom::Motion m)) {
  auto const t = *rc::gen::nonNegative<double>();

  RC_PRE(!boost::empty(m.effector_names()));

  auto frame = m.new_keyframe();
  auto [k, e] = *frame.effectors().begin();
  if (e.location()) {
    e.clear_location();
  } else {
    e.set_location(flom::Location{});
  }
  frame.set_effector(k, e);
  RC_ASSERT_THROWS_AS(m.insert_keyframe(t, frame),
                      flom::errors::InvalidFrameError);
}

RC_BOOST_PROP(delete_init_keyframe, (flom::Motion m)) {
  RC_ASSERT_THROWS_AS(m.delete_keyframe(0), flom::errors::InitKeyframeError);
}

RC_BOOST_PROP(delete_keyframe, (flom::Motion m)) {
  auto const t = *rc::gen::positive<double>();

  auto const frame = m.new_keyframe();
  m.insert_keyframe(t, frame);
  m.delete_keyframe(t);

  RC_ASSERT_THROWS_AS(m.delete_keyframe(t),
                      flom::errors::KeyframeNotFoundError);
}

BOOST_AUTO_TEST_SUITE_END()
