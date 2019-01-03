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

#include "flom/effector.hpp"
#include "flom/constants.hpp"
#include "flom/interpolation.hpp"

#include <cmath>
#include <limits>
#include <utility>

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_traits_array.hpp>
#include <boost/test/floating_point_comparison.hpp>

namespace flom {

Effector &Effector::repeat(std::size_t n) {
  if (this->location) {
    this->location->vec *= n;
  }
  if (this->rotation) {
    // TODO: Don't call normalize here
    boost::qvm::normalize(this->rotation->quat);
    for (std::size_t i = 0; i < n; i++) {
      this->rotation->quat *= this->rotation->quat;
    }
  }
  return *this;
}

Effector &Effector::compose(const Effector &other) {
  if (this->location && other.location) {
    this->location->vec += other.location->vec;
  }
  if (this->rotation && other.rotation) {
    // TODO: Don't call normalize here
    boost::qvm::normalize(this->rotation->quat);
    this->rotation->quat *= boost::qvm::normalized(other.rotation->quat);
  }
  return *this;
}

Effector Effector::repeated(std::size_t n) const {
  Effector copy{*this};
  return copy.repeat(n);
}

Effector Effector::composed(const Effector &other) const {
  Effector copy{*this};
  return copy.compose(other);
}

Effector interpolate(double t, Effector const &a, Effector const &b) {
  Effector e;
  if (a.rotation && b.rotation) {
    e.rotation = interpolate(t, *a.rotation, *b.rotation);
  }
  if (a.location && b.location) {
    e.location = interpolate(t, *a.location, *b.location);
  }
  return e;
}

Rotation interpolate(double t, Rotation const &a, Rotation const &b) {
  Rotation result;

  // TODO: Check possibility of NaNs before calling qvm::slerp
  result.quat = qvm::slerp(a.quat, b.quat, t);
  if (std::isnan(qvm::S(result.quat))) {
    result.quat = a.quat;
  }

  result.weight = lerp(t, a.weight, b.weight);
  return result;
}

bool operator==(const Rotation &r1, const Rotation &r2) {
  return r1.weight == r2.weight && r1.quat == r2.quat;
}

bool operator!=(const Rotation &r1, const Rotation &r2) { return !(r1 == r2); }

bool almost_equal(const Rotation &r1, const Rotation &r2) {
  return almost_equal(r1.weight, r2.weight) &&
         boost::qvm::cmp(r1.quat, r2.quat,
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
}

Location interpolate(double t, Location const &a, Location const &b) {
  Location result;
  result.vec = lerp(t, a.vec, b.vec);
  result.weight = lerp(t, a.weight, b.weight);
  return result;
}

bool operator==(const Location &l1, const Location &l2) {
  return l1.weight == l2.weight && l1.vec == l2.vec;
}

bool operator!=(const Location &l1, const Location &l2) { return !(l1 == l2); }

bool almost_equal(const Location &l1, const Location &l2) {
  return almost_equal(l1.weight, l2.weight) &&
         boost::qvm::cmp(l1.vec, l2.vec,
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
}

bool operator==(const Effector &e1, const Effector &e2) {
  return e1.location == e2.location && e1.rotation == e2.rotation;
}

bool operator!=(const Effector &e1, const Effector &e2) { return !(e1 == e2); }

bool almost_equal(const Effector &e1, const Effector &e2) {
  // TODO: Refactor: Remove mutable variable
  const bool l =
      static_cast<bool>(e1.location) == static_cast<bool>(e2.location);
  const bool r =
      static_cast<bool>(e1.rotation) == static_cast<bool>(e2.rotation);
  if (!l || !r)
    return false;
  bool result = true;
  if (e1.location)
    result = result && almost_equal(*e1.location, *e2.location);
  if (e1.rotation)
    result = result && almost_equal(*e1.rotation, *e2.rotation);
  return result;
}

double interpolate(double t, double a, double b) { return lerp(t, a, b); }
bool almost_equal(double a, double b) {
  return boost::math::fpc::close_at_tolerance<double>(
      constants::float_point_tolerance)(a, b);
}

} // namespace flom
