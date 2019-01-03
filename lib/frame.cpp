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
#include "flom/interpolation.hpp"

#include <boost/range/adaptors.hpp>

#include <algorithm>

namespace flom {

Frame &Frame::repeat(std::size_t n) {
  for (auto &&[k, e] : this->effectors) {
    e.repeat(n);
  }
  return *this;
}

Frame &Frame::compose(const Frame &other) {
  for (auto &&[k, e] : this->effectors) {
    auto const o = other.effectors.at(k);
    e.compose(o);
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

bool operator==(const Frame &f1, const Frame &f2) {
  return f1.positions == f2.positions && f1.effectors == f2.effectors;
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
