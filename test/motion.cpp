#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include <cmath>
#include <fstream>

#include <flom/motion.hpp>

#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion)

RC_BOOST_PROP(retrieve_frame_wrap, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_PRE(m.is_valid());

  auto len = m.length();
  auto mul = static_cast<unsigned>(t / len);
  auto frame = m.frame_at(t);
  auto expected_frame = m.frame_at(std::fmod(t, len)) + m.frame_at(len) * mul;

  RC_ASSERT(flom::almost_equal(frame, expected_frame));
}

RC_BOOST_PROP(retrieve_frame_none, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.length() >= t);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_PRE(m.is_valid());

  auto len = m.length();
  auto mul = static_cast<unsigned>(t / len);
  auto frame = m.frame_at(t);
  auto expected_frame = m.frame_at(std::fmod(t, len)) + m.frame_at(len) * mul;

  RC_ASSERT(flom::almost_equal(frame, expected_frame));
}

RC_BOOST_PROP(retrieve_frame_none_throw, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.length() < t);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_PRE(m.is_valid());

  RC_ASSERT_THROWS(m.frame_at(t));
}

BOOST_AUTO_TEST_SUITE_END()
