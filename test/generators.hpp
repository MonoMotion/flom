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
        [](std::string const &model_id, flom::LoopType loop, double fps, unsigned length,
           auto const &joint_names, auto const& effector_names) {
          flom::Motion m(model_id);
          m.set_loop(loop);
          for(unsigned i = 0; i < length; ++i) {
            auto& f = m.get_or_insert_frame(i * fps);
            std::transform(std::cbegin(joint_names), std::cend(joint_names), std::inserter(f.positions, std::end(f.positions)), [](auto&& n) {
                const double p = static_cast<double>(*gen::inRange(-157, 157)) / 100;
                return std::make_pair(n, p);
            });
            std::transform(std::cbegin(effector_names), std::cend(effector_names), std::inserter(f.effectors, std::end(f.effectors)), [](auto&& n) {
                auto const e = *gen::arbitrary<flom::Effector>();
                return std::make_pair(n, e);
            });
          }
          return m;
        },
        gen::arbitrary<std::string>(),
        gen::element(flom::LoopType::None, flom::LoopType::Wrap),
        gen::arbitrary<double>(),
        gen::arbitrary<unsigned>(),
        gen::arbitrary<std::vector<std::string>>(),
        gen::arbitrary<std::vector<std::string>>());
  }
};

} // namespace rc

#endif
