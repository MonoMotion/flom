#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include "generators.hpp"

BOOST_AUTO_TEST_SUITE(effector)

RC_BOOST_PROP(mul_scalar_location,
              (const from::Effector& e, double v)) {
  RC_PRE(e.location);

  auto e2 = e * v;
  RC_ASSERT(e2.location->vec == e.location->vec * v);
}

RC_BOOST_PROP(mul_scalar_rotation,
              (const from::Effector& e, double v)) {
  RC_PRE(e.rotation);

  auto e2 = e * v;
  RC_ASSERT(e2.rotation->quat == e.rotation->quat * v);
}

BOOST_AUTO_TEST_SUITE_END()
