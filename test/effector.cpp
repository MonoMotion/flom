#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include "generators.hpp"

BOOST_AUTO_TEST_SUITE(effector)

BOOST_AUTO_TEST_CASE(test1) {
  const int x = 1;
  BOOST_CHECK(x == 1);
}

BOOST_AUTO_TEST_SUITE_END()
