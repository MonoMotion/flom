#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>

#include <flom/motion.hpp>

#include "generators.hpp"
#include "operators.hpp"
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

  // Using non-strict version of operator== defined in operators.hpp
  RC_ASSERT(frame == expected_frame);
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

  // Using non-strict version of operator== defined in operators.hpp
  RC_ASSERT(frame == expected_frame);
}

RC_BOOST_PROP(retrieve_frame_none_throw, (const flom::Motion &m)) {
  RC_PRE(m.is_valid());
  RC_PRE(m.loop() == flom::LoopType::None);

  // Using unsigned long because RapidCheck doesn't support gen::inRange<double>
  RC_PRE(m.length() < std::numeric_limits<unsigned long>::max());
  unsigned long lower = m.length() + 1;
  unsigned long upper = std::numeric_limits<unsigned long>::max();
  double t = *rc::gen::inRange<unsigned long>(lower, upper);

  RC_ASSERT_THROWS(m.frame_at(t));
}

RC_BOOST_PROP(frames_range_none, (const flom::Motion &m, double fps)) {
  RC_PRE(fps > 0);
  RC_PRE(m.length() >= fps);
  RC_PRE(m.loop() == flom::LoopType::None);
  RC_PRE(m.is_valid());

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
  RC_PRE(m.is_valid());

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
  RC_PRE(m.is_valid());

  RC_ASSERT((m.length() >= t) == m.is_in_range_at(t));
}

RC_BOOST_PROP(in_range_t_wrap, (const flom::Motion &m, double t)) {
  RC_PRE(t >= 0);
  RC_PRE(m.loop() == flom::LoopType::Wrap);
  RC_PRE(m.is_valid());

  RC_ASSERT(m.is_in_range_at(t));
}

RC_BOOST_PROP(dump_load, (const flom::Motion &m)) {
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
