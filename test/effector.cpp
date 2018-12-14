#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include "generators.hpp"

BOOST_AUTO_TEST_SUITE(effector)

RC_BOOST_PROP(mul_scalar_location, (const flom::Effector &e, double v)) {
  RC_PRE(e.location);

  auto e2 = e * v;
  RC_ASSERT(e2.location->vec == e.location->vec * v);
}

RC_BOOST_PROP(mul_scalar_rotation, (const flom::Effector &e, double v)) {
  RC_PRE(e.rotation);

  auto e2 = e * v;
  RC_ASSERT(e2.rotation->quat == e.rotation->quat * v);
}

RC_BOOST_PROP(add_location,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.location && e2.location);

  auto e3 = e1 + e2;
  RC_ASSERT(e3.location->vec == e1.location->vec + e2.location->vec);
}

RC_BOOST_PROP(add_rotation,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.rotation && e2.rotation);

  auto e3 = e1 + e2;
  RC_ASSERT(e3.rotation->quat == e1.rotation->quat + e2.rotation->quat);
}

RC_BOOST_PROP(sub_location,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.location && e2.location);

  auto e3 = e1 - e2;
  RC_ASSERT(e3.location->vec == e1.location->vec - e2.location->vec);
}

RC_BOOST_PROP(sub_rotation,
              (const flom::Effector &e1, const flom::Effector &e2)) {
  RC_PRE(e1.rotation && e2.rotation);

  auto e3 = e1 - e2;
  RC_ASSERT(e3.rotation->quat == e1.rotation->quat - e2.rotation->quat);
}

BOOST_AUTO_TEST_SUITE_END()
