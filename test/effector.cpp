#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck/boost_test.h>

BOOST_AUTO_TEST_CASE(test1) {
  const int x = 1;
  BOOST_CHECK(x == 1);
}
