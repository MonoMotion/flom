#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include <flom/frame.hpp>
#include <flom/interpolation.hpp>

#include "generators.hpp"
#include "operators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(frame)

RC_BOOST_PROP(mul_scalar, (const flom::Frame &f, double v)) {
  auto f2 = f * v;
  for (auto &&[key, val] : f.positions) {
    RC_ASSERT(f2.positions.at(key) == val * v);
  }
  for (auto &&[key, val] : f.effectors) {
    RC_ASSERT(f2.effectors.at(key) == val * v);
  }
}

RC_BOOST_PROP(add, (const flom::Frame &f1)) {
  auto f2 = f1;
  auto const f3 = f1 + f2;
  for (auto &&[key, val] : f3.positions) {
    RC_ASSERT(val == f1.positions.at(key) + f2.positions.at(key));
  }
  for (auto &&[key, val] : f3.effectors) {
    RC_ASSERT(val == f1.effectors.at(key) + f2.effectors.at(key));
  }
}

RC_BOOST_PROP(sub, (const flom::Frame &f1)) {
  auto f2 = f1;
  auto const f3 = f1 - f2;
  for (auto &&[key, val] : f3.positions) {
    RC_ASSERT(val == f1.positions.at(key) - f2.positions.at(key));
  }
  for (auto &&[key, val] : f3.effectors) {
    RC_ASSERT(val == f1.effectors.at(key) - f2.effectors.at(key));
  }
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
