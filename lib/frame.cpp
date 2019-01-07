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

#include "flom/frame.hpp"
#include "flom/effector_type.hpp"
#include "flom/interpolation.hpp"

#include <boost/range/adaptors.hpp>

#include <algorithm>

namespace flom {

FrameDifference operator-(const Frame &f1, const Frame &f2) {
  return FrameDifference{f1, f2};
}

FrameDifference::FrameDifference(const Frame &f1, const Frame &f2) {
  for (auto const &[k, e] : f1.effectors) {
    auto const o = f2.effectors.at(k);
    this->effectors.emplace(k, e - o);
  }
  for (auto const &[k, p] : f1.positions) {
    auto const o = f2.positions.at(k);
    this->positions.emplace(k, p - o);
  }
}

FrameDifference::FrameDifference(
    const std::unordered_map<std::string, EffectorType> &types, const Frame &f1,
    const Frame &f2) {
  for (auto const &[k, e] : f1.effectors) {
    auto const o = f2.effectors.at(k);
    auto const type = types.at(k);
    this->effectors.emplace(k, EffectorDifference{type, e, o});
  }
  for (auto const &[k, p] : f1.positions) {
    auto const o = f2.positions.at(k);
    this->positions.emplace(k, p - o);
  }
}

FrameDifference &FrameDifference::operator*=(std::size_t n) {
  for (auto &&[k, e] : this->effectors) {
    e *= n;
  }
  return *this;
}

FrameDifference &FrameDifference::operator+=(const FrameDifference &other) {
  for (auto &&[k, e] : this->effectors) {
    auto const o = other.effectors.at(k);
    e += o;
  }
  return *this;
}

Frame &Frame::operator+=(const FrameDifference &other) {
  for (auto &&[k, e] : this->effectors) {
    auto const o = other.effectors.at(k);
    e += o;
  }
  return *this;
}

Frame interpolate(double t, Frame const &a, Frame const &b) {
  Frame f;
  for (auto const &[k, v1] : a.positions) {
    auto const v2 = b.positions.at(k);
    f.positions[k] = lerp(t, v1, v2);
  }
  for (auto const &[k, v1] : a.effectors) {
    auto const v2 = b.effectors.at(k);
    f.effectors[k] = interpolate(t, v1, v2);
  }
  return f;
}

Frame Frame::new_compatible_frame() const {
  Frame copy{*this};
  for (auto &&[k, v] : copy.positions) {
    v = 0.0;
  }
  for (auto &&[k, v] : copy.effectors) {
    v = Effector{};
  }
  return copy;
}

bool operator==(const FrameDifference &d1, const FrameDifference &d2) {
  return d1.positions == d2.positions && d1.effectors == d2.effectors;
}

bool operator==(const Frame &f1, const Frame &f2) {
  return f1.positions == f2.positions && f1.effectors == f2.effectors;
}

bool operator!=(const Frame &f1, const Frame &f2) { return !(f1 == f2); }

bool almost_equal(const FrameDifference &f1, const FrameDifference &f2) {
  auto p = std::all_of(std::cbegin(f1.positions), std::cend(f1.positions),
                       [&f2](auto const &pair) {
                         auto const &[joint, pos1] = pair;
                         auto const pos2 = f2.positions.at(joint);
                         return almost_equal(pos1, pos2);
                       });
  auto e = std::all_of(std::cbegin(f1.effectors), std::cend(f1.effectors),
                       [&f2](auto const &pair) {
                         auto const &[link, e1] = pair;
                         auto const e2 = f2.effectors.at(link);
                         return almost_equal(e1, e2);
                       });
  return p && e;
}

bool almost_equal(const Frame &f1, const Frame &f2) {
  auto p = std::all_of(std::cbegin(f1.positions), std::cend(f1.positions),
                       [&f2](auto const &pair) {
                         auto const &[joint, pos1] = pair;
                         auto const pos2 = f2.positions.at(joint);
                         return almost_equal(pos1, pos2);
                       });
  auto e = std::all_of(std::cbegin(f1.effectors), std::cend(f1.effectors),
                       [&f2](auto const &pair) {
                         auto const &[link, e1] = pair;
                         auto const e2 = f2.effectors.at(link);
                         return almost_equal(e1, e2);
                       });
  return p && e;
}

KeyRange<std::string> Frame::joint_names() const {
  return this->positions | boost::adaptors::map_keys;
}

KeyRange<std::string> Frame::effector_names() const {
  return this->effectors | boost::adaptors::map_keys;
}

} // namespace flom
