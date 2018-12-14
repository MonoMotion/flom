#ifndef FLOM_TEST_GENERATORS_HPP
#define FLOM_TEST_GENERATORS_HPP

#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec.hpp>

#include <rapidcheck.h>

#include <flom/effector.hpp>
#include <flom/frame.hpp>
#include <flom/motion.hpp>

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
          boost::qvm::set_rotx(q, static_cast<double>(x) / 100);
          boost::qvm::set_roty(q, static_cast<double>(y) / 100);
          boost::qvm::set_rotz(q, static_cast<double>(z) / 100);
          return q;
        },
        gen::inRange(-157, 157), gen::inRange(-157, 157),
        gen::inRange(-157, 157));
  };
};

template <> struct Arbitrary<flom::Location> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](unsigned weight, flom::CoordinateSystem c,
           boost::qvm::vec<double, 3> const &v) {
          flom::Location l;
          l.weight = static_cast<double>(weight) / 100;
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
          r.weight = static_cast<double>(weight) / 100;
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

template <> struct Arbitrary<flom::Motion> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](std::string const &model_id, flom::LoopType loop,
           auto const &frames) {
          flom::Motion m(model_id);
          m.set_loop(loop);
          for (auto const &[t, f] : frames) {
            m.get_or_insert_frame(t) = f;
          }
          return m;
        },
        gen::arbitrary<std::string>(),
        gen::element(flom::LoopType::None, flom::LoopType::Wrap),
        gen::arbitrary<std::unordered_map<double, flom::Frame>>());
  }
};

} // namespace rc

#endif
