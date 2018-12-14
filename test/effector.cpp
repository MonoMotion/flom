#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#include <flom/effector.hpp>
#include <flom/frame.hpp>

#include <unordered_map>

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

template <> struct Arbitrary<flom::Location> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](unsigned weight, flom::CoordinateSystem c,
           boost::qvm::vec<double, 3> const &v) {
          flom::Location l;
          l.weight = weight / 100;
          l.coord_system = c;
          l.vec = v;
          return l;
        },
        gen::inRange<unsigned>(0, 100),
        gen::element(flom::CoordinateSystem::World,
                     flom::CoordinateSystem::Local),
        gen::arbitrary<boost::qvm::vec<double, 3>>());
  }
};

template <> struct Arbitrary<flom::Rotation> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](unsigned weight, flom::CoordinateSystem c,
           boost::qvm::quat<double> const &q) {
          flom::Rotation r;
          r.weight = weight / 100;
          r.coord_system = c;
          r.quat = q;
          return r;
        },
        gen::inRange<unsigned>(0, 100),
        gen::element(flom::CoordinateSystem::World,
                     flom::CoordinateSystem::Local),
        gen::arbitrary<boost::qvm::quat<double>>());
  }
};

template <> struct Arbitrary<flom::Effector> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](Maybe<flom::Location> const &l, Maybe<flom::Rotation> const &r) {
          flom::Effector e;
          if (l) {
            e.location = *l;
          }
          if (r) {
            e.rotation = *r;
          }
          return e;
        },
        gen::maybe(gen::arbitrary<flom::Location>()),
        gen::maybe(gen::arbitrary<flom::Rotation>()));
  }
};

template <> struct Arbitrary<flom::Frame> {
  static auto arbitrary() -> decltype(auto) {
    return gen::build(
        gen::construct<flom::Frame>(),
        gen::set(&flom::Frame::positions,
                 gen::arbitrary<std::unordered_map<std::string, double>>()),
        gen::set(
            &flom::Frame::effectors,
            gen::arbitrary<std::unordered_map<std::string, flom::Effector>>()));
  }
};

} // namespace rc

BOOST_AUTO_TEST_SUITE(effector)

BOOST_AUTO_TEST_CASE(test1) {
  const int x = 1;
  BOOST_CHECK(x == 1);
}

BOOST_AUTO_TEST_SUITE_END()
