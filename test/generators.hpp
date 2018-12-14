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
        [](std::string const &model_id, flom::LoopType loop, double fps, auto const& frames) {
          flom::Motion m(model_id);
          m.set_loop(loop);
          unsigned i = 0;
          for (auto const& [p, e] : frames) {
            auto& f = m.get_or_insert_frame(i++ * fps);
            std::copy(std::cbegin(p), std::cend(p), std::inserter(f.positions, std::end(f.positions)));
            std::copy(std::cbegin(e), std::cend(e), std::inserter(f.effectors, std::end(f.effectors)));
          }
          return m;
        },
        gen::arbitrary<std::string>(),
        gen::element(flom::LoopType::None, flom::LoopType::Wrap),
        gen::arbitrary<double>(),
        gen::mapcat(
           gen::pair(
            gen::arbitrary<std::vector<std::string>>(),
            gen::arbitrary<std::vector<std::string>>()
          ),
          [] (auto const& t) {
            auto const& [joints, effectors] = t;
            auto j = gen::exec([&joints = joints]() {
              std::unordered_map<std::string, double> nj;
              std::transform(std::cbegin(joints), std::cend(joints), std::inserter(nj, std::end(nj)), [](auto&& j) {
                  return std::make_pair(j, *gen::arbitrary<double>());
              });
              return nj;
            });
            auto e = gen::exec([&effectors = effectors]() {
              std::unordered_map<std::string, flom::Effector> nj;
              std::transform(std::cbegin(effectors), std::cend(effectors), std::inserter(nj, std::end(nj)), [](auto&& j) {
                  return std::make_pair(j, *gen::arbitrary<flom::Effector>());
              });
              return nj;
            });
            return gen::container<std::vector<std::pair<std::unordered_map<std::string, double>, std::unordered_map<std::string, flom::Effector>>>>(gen::pair(j, e));
          }
        ));
  }
};

} // namespace rc

#endif
