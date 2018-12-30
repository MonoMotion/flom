//
// Copyright 2018 coord.e
//
// This file is part of Flom.
//
// Flom is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Flom is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Flom.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef FLOM_TEST_GENERATORS_HPP
#define FLOM_TEST_GENERATORS_HPP

#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec.hpp>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

#include <rapidcheck.h>

#include <flom/effector.hpp>
#include <flom/frame.hpp>
#include <flom/motion.hpp>
#include <flom/constants.hpp>

#include <unordered_map>

namespace rc {

static constexpr int half_pi_100 = static_cast<int>(flom::constants::pi<double> / 2 * 100);

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
          boost::qvm::set_identity(q);
          boost::qvm::rotate_x(q, static_cast<double>(x) / 100);
          boost::qvm::rotate_y(q, static_cast<double>(y) / 100);
          boost::qvm::rotate_z(q, static_cast<double>(z) / 100);
          return q;
        },
        gen::inRange(-half_pi_100, half_pi_100), gen::inRange(-half_pi_100, half_pi_100),
        gen::inRange(-half_pi_100, half_pi_100));
  }
};

template <> struct Arbitrary<flom::Location> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](unsigned weight,
           boost::qvm::vec<double, 3> const &v) {
          flom::Location l;
          l.weight = static_cast<double>(weight) / 100;
          l.vec = v;
          return l;
        },
        gen::inRange<unsigned>(0, 100),
        gen::arbitrary<boost::qvm::vec<double, 3>>());
  }
};

template <> struct Arbitrary<flom::Rotation> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](unsigned weight,
           boost::qvm::quat<double> const &q) {
          flom::Rotation r;
          r.weight = static_cast<double>(weight) / 100;
          r.quat = q;
          return r;
        },
        gen::inRange<unsigned>(0, 100),
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
          std::unordered_set<std::string> joint_names, effector_names;
          auto const& init_frame = frames[0];
          boost::copy(init_frame.first | boost::adaptors::map_keys, std::inserter(joint_names, std::end(joint_names)));
          boost::copy(init_frame.second | boost::adaptors::map_keys, std::inserter(effector_names, std::end(effector_names)));
          flom::Motion m(joint_names, effector_names, model_id);
          m.set_loop(loop);
          unsigned i = 0;
          for (auto const& [p, e] : frames) {
            auto& f = m.get_or_insert_frame(fps * i++);
            std::copy(std::cbegin(p), std::cend(p), std::inserter(f.positions, std::end(f.positions)));
            std::copy(std::cbegin(e), std::cend(e), std::inserter(f.effectors, std::end(f.effectors)));
          }
          return m;
        },
        // UTF8 string is required
        gen::nonEmpty(gen::container<std::string>(gen::inRange('a', 'z'))),
        gen::element(flom::LoopType::None, flom::LoopType::Wrap),
        gen::nonZero<double>(),
        gen::nonEmpty(gen::mapcat(
           gen::pair(
             gen::container<std::vector<std::string>>(gen::container<std::string>(gen::inRange('a', 'z'))),
             gen::container<std::vector<std::string>>(gen::container<std::string>(gen::inRange('a', 'z')))
          ),
          [] (auto const& t) {
            auto const& [joints, effectors] = t;
            auto j = gen::exec([&joints = joints]() {
              std::unordered_map<std::string, double> nj;
              std::transform(std::cbegin(joints), std::cend(joints), std::inserter(nj, std::end(nj)), [](auto&& name) {
                  return std::make_pair(name, static_cast<double>(*gen::inRange(-half_pi_100, half_pi_100)) / 100);
              });
              return nj;
            });
            auto eg = gen::build(
                gen::construct<flom::Effector>(),
                gen::set(&flom::Effector::location, gen::arbitrary<flom::Location>()),
                gen::set(&flom::Effector::rotation, gen::arbitrary<flom::Rotation>()));
            auto e = gen::exec([&effectors = effectors, eg]() {
              std::unordered_map<std::string, flom::Effector> nj;
              std::transform(std::cbegin(effectors), std::cend(effectors), std::inserter(nj, std::end(nj)), [&eg](auto&& name) {
                  return std::make_pair(name, *eg);
              });
              return nj;
            });
            return gen::container<std::vector<std::pair<std::unordered_map<std::string, double>, std::unordered_map<std::string, flom::Effector>>>>(gen::pair(j, e));
          }
        )));
  }
};

} // namespace rc

#endif
