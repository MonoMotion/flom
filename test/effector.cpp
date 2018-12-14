#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

namespace rc {

template <> struct Arbitrary<boost::qvm::vec<double, 3>> {
  static auto arbitrary() -> decltype(auto) {
    return gen::construct<boost::qvm::vec<double, 3>>(gen::arbitrary<double>(),
                                                      gen::arbitrary<double>(),
                                                      gen::arbitrary<double>());
  }
};

template <> struct Arbitrary<boost::qvm::quat<double>> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](int x, int y, int z) {
          auto q = boost::qvm::quat<double>();
          boost::qvm::set_rotx(q, x / 100);
          boost::qvm::set_roty(q, y / 100);
          boost::qvm::set_rotz(q, z / 100);
          return q;
        },
        gen::inRange(-157, 157), gen::inRange(-157, 157),
        gen::inRange(-157, 157));
  }
};

} // namespace rc

BOOST_AUTO_TEST_SUITE(effector)

BOOST_AUTO_TEST_CASE(test1) {
  const int x = 1;
  BOOST_CHECK(x == 1);
}

BOOST_AUTO_TEST_SUITE_END()
