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

FrameDifference operator-(const Frame &f1, const Frame &f2) {
  return FrameDifference{f1, f2};
}

FrameDifference::FrameDifference(const Frame &f1, const Frame &f2) {
  // Not throwing exception in favor of better performance
  assert(f1.is_compatible(f2) &&
         "Cannot perform the operation on Incompatible frames");

  for (auto const &[k, e] : f1.effectors()) {
    auto const o = f2.effectors().at(k);
    this->effectors_.emplace(k, e - o);
  }
  for (auto const &[k, p] : f1.positions()) {
    auto const o = f2.positions().at(k);
    this->positions_.emplace(k, p - o);
  }
}

const std::unordered_map<std::string, double> &
FrameDifference::positions() const & {
  return this->positions_;
}
std::unordered_map<std::string, double> FrameDifference::positions() && {
  return std::move(this->positions_);
}

const std::unordered_map<std::string, EffectorDifference> &
FrameDifference::effectors() const & {
  return this->effectors_;
}
std::unordered_map<std::string, EffectorDifference>
FrameDifference::effectors() && {
  return std::move(this->effectors_);
}

FrameDifference &FrameDifference::operator*=(std::size_t n) {
  for (auto &&[k, p] : this->positions_) {
    p *= n;
  }
  for (auto &&[k, e] : this->effectors_) {
    e *= n;
  }
  return *this;
}

FrameDifference &FrameDifference::operator+=(const FrameDifference &other) {
  // Not throwing exception in favor of better performance
  assert(this->is_compatible(other) &&
         "Cannot use an incompatible FrameDifference instance");

  for (auto &&[k, p] : this->positions_) {
    auto const o = other.positions_.at(k);
    p += o;
  }
  for (auto &&[k, e] : this->effectors_) {
    auto const o = other.effectors_.at(k);
    e += o;
  }
  return *this;
}

Frame &Frame::operator+=(const FrameDifference &other) {
  // Not throwing exception in favor of better performance
  assert(this->is_compatible(other) &&
         "Cannot use an incompatible FrameDifference instance");

  for (auto &&[k, p] : this->positions_) {
    auto const o = other.positions().at(k);
    p += o;
  }
  for (auto &&[k, e] : this->effectors_) {
    auto const o = other.effectors().at(k);
    e += o;
  }
  return *this;
}

Frame interpolate(double t, Frame const &a, Frame const &b) {
  // Not throwing exception in favor of better performance
  assert(a.is_compatible(b) &&
         "Cannot perform the operation on Incompatible frames");

  Frame f;
  for (auto const &[k, v1] : a.positions()) {
    auto const v2 = b.positions().at(k);
    f.set_position(k, lerp(t, v1, v2));
  }
  for (auto const &[k, v1] : a.effectors()) {
    auto const v2 = b.effectors().at(k);
    f.set_effector(k, interpolate(t, v1, v2));
  }
  return f;
}

Frame Frame::new_compatible_frame() const {
  Frame copy{*this};
  for (auto &&[k, v] : copy.positions_) {
    v = 0.0;
  }
  for (auto &&[k, v] : copy.effectors_) {
    Effector e{v.new_compatible_effector()};
    v = e;
  }
  return copy;
}

bool FrameDifference::is_compatible(const FrameDifference &other) const {
  for (auto const &[k, v] : this->effectors()) {
    auto const &o = other.effectors().at(k);
    if (!v.is_compatible(o)) {
      return false;
    }
  }

  return true;
}
bool Frame::is_compatible(const FrameDifference &other) const {
  for (auto const &[k, v] : this->effectors()) {
    auto const &o = other.effectors().at(k);
    if (!v.is_compatible(o)) {
      return false;
    }
  }

  return true;
}
bool Frame::is_compatible(const Frame &other) const {
  for (auto const &[k, v] : this->effectors()) {
    auto const &o = other.effectors().at(k);
    if (!v.is_compatible(o)) {
      return false;
    }
  }

  return true;
}

bool operator==(const FrameDifference &d1, const FrameDifference &d2) {
  return d1.positions() == d2.positions() && d1.effectors() == d2.effectors();
}

bool operator==(const Frame &f1, const Frame &f2) {
  return f1.positions() == f2.positions() && f1.effectors() == f2.effectors();
}

bool operator!=(const Frame &f1, const Frame &f2) { return !(f1 == f2); }

Frame::Frame() = default;
Frame::Frame(const Frame::PositionsMap &positions,
             const Frame::EffectorsMap &effectors)
    : positions_(positions), effectors_(effectors) {}

const Frame::PositionsMap &Frame::positions() const & {
  return this->positions_;
}
Frame::PositionsMap Frame::positions() && {
  return std::move(this->positions_);
}

void Frame::set_positions(const Frame::PositionsMap &positions) {
  this->positions_ = positions;
}

void Frame::set_position(const std::string &name, double v) {
  this->positions_[name] = v;
}

const Frame::EffectorsMap &Frame::effectors() const & {
  return this->effectors_;
}
Frame::EffectorsMap Frame::effectors() && {
  return std::move(this->effectors_);
}

void Frame::set_effectors(const Frame::EffectorsMap &effectors) {
  this->effectors_ = effectors;
}

void Frame::set_effector(const std::string &name, const Effector &v) {
  this->effectors_[name] = v;
}

KeyRange<std::string> Frame::joint_names() const {
  return this->positions_ | boost::adaptors::map_keys;
}

KeyRange<std::string> Frame::effector_names() const {
  return this->effectors_ | boost::adaptors::map_keys;
}

} // namespace flom
