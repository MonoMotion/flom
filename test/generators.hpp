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

#include <boost/range/combine.hpp>
#include <boost/range/algorithm.hpp>

#include <rapidcheck.h>

#include <flom/effector.hpp>
#include <flom/frame.hpp>
#include <flom/motion.hpp>
#include <flom/constants.hpp>

#include <unordered_map>
#include <unordered_set>

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
    return gen::build(gen::construct<flom::Location>(),
                      gen::set(&flom::Location::vec, gen::arbitrary<boost::qvm::vec<double, 3>>()));
  }
};

template <> struct Arbitrary<flom::Rotation> {
  static auto arbitrary() -> decltype(auto) {
    return gen::build(gen::construct<flom::Rotation>(),
                      gen::set(&flom::Rotation::quat, gen::arbitrary<boost::qvm::quat<double>>()));
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

template <> struct Arbitrary<flom::CoordinateSystem> {
  static auto arbitrary() -> decltype(auto) {
    return gen::element(flom::CoordinateSystem::World, flom::CoordinateSystem::Local);
  }
};

template <> struct Arbitrary<flom::EffectorType> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](Maybe<flom::CoordinateSystem> l, Maybe<flom::CoordinateSystem> r) {
          std::optional<flom::CoordinateSystem> loc, rot;
          if (l) {
            loc = *l;
          }
          if (r) {
            rot = *r;
          }
          return flom::EffectorType {loc, rot};
        },
        gen::maybe(gen::arbitrary<flom::CoordinateSystem>()),
        gen::maybe(gen::arbitrary<flom::CoordinateSystem>()));
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

template <> struct Arbitrary<flom::EffectorDifference> {
  static auto arbitrary() -> decltype(auto) {
    auto effector_gen = gen::build(
        gen::construct<flom::Effector>(),
        gen::set(&flom::Effector::location, gen::arbitrary<flom::Location>()),
        gen::set(&flom::Effector::rotation, gen::arbitrary<flom::Rotation>())
      );
    return gen::apply(
        [](const auto& e1, const auto& e2) {
          return e1 - e2;
        },
        effector_gen,
        effector_gen
        );
  }
};


template <> struct Arbitrary<flom::FrameDifference> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](const auto& f) {
          auto const empty = f.new_compatible_frame();
          return f - empty;
        },
        gen::arbitrary<flom::Frame>()
      );
  }
};

flom::Effector convert_effector(const flom::EffectorType& type, flom::Effector e) {
  // Convert the effector to be compatible with the EffectorType
  if (!type.location()) {
    e.location = std::nullopt;
  } else {
    if (!e.location) {
      e.location = flom::Location {};
    }
  }

  if (!type.rotation()) {
    e.rotation = std::nullopt;
  } else {
    if (!e.rotation) {
      e.rotation = flom::Rotation {};
    }
  }

  return e;
}

template <> struct Arbitrary<flom::Motion> {
  static auto arbitrary() -> decltype(auto) {
    return gen::apply(
        [](std::string const &model_id, flom::LoopType loop, double fps, auto const& t) {
          auto const& [joint_names, effector_types, frames] = t;
          flom::Motion m(joint_names, effector_types, model_id);
          m.set_loop(loop);
          unsigned i = 0;
          for (auto const& [p, e] : frames) {
            flom::Frame f;
            for(auto const& pair : boost::combine(joint_names, p)) {
              f.positions.emplace(boost::get<0>(pair), boost::get<1>(pair));
            }
            for(auto const& pair : boost::combine(effector_types, e)) {
              auto const& [name, type] = boost::get<0>(pair);
              auto const& eff = boost::get<1>(pair);
              f.effectors.emplace(name, convert_effector(type, eff));
            }
            m.insert_keyframe(fps * i++, f);
          }
          return m;
        },
        // UTF8 string is required
        gen::nonEmpty(gen::container<std::string>(gen::inRange('a', 'z'))),
        gen::element(flom::LoopType::None, flom::LoopType::Wrap),
        gen::positive<double>(),
        gen::mapcat(
           gen::pair(
             gen::inRange<std::size_t>(0, 1e3),
             gen::inRange<std::size_t>(0, 1e3)
          ),
          [](auto const& t) {
            auto const [num_joints, num_effectors] = t;

            auto str_gen = gen::container<std::string>(gen::inRange('a', 'z'));
            auto joint_names_gen = gen::container<std::unordered_set<std::string>>(num_joints, str_gen);
            auto effector_names_gen = gen::container<std::unordered_map<std::string, flom::EffectorType>>(num_effectors, str_gen, gen::arbitrary<flom::EffectorType>());

            auto position_gen = gen::map(gen::inRange(-half_pi_100, half_pi_100),
                [](auto i){ return static_cast<double>(i) / 100; }
              );
            auto positions_gen = gen::container<std::vector<double>>(num_joints, position_gen);

            auto effector_gen = gen::build(
                gen::construct<flom::Effector>(),
                gen::set(&flom::Effector::location, gen::arbitrary<flom::Location>()),
                gen::set(&flom::Effector::rotation, gen::arbitrary<flom::Rotation>())
              );
            auto effectors_gen = gen::container<std::vector<flom::Effector>>(num_effectors, effector_gen);

            auto frames_gen = gen::nonEmpty(gen::container<std::vector<std::pair<std::vector<double>, std::vector<flom::Effector>>>>(gen::pair(positions_gen, effectors_gen)));
            return gen::tuple(joint_names_gen, effector_names_gen, frames_gen);
        }));
  }
};

} // namespace rc

#endif
