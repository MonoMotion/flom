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

EffectorDifference operator-(const Effector &e1, const Effector &e2) {
  return EffectorDifference{e1, e2};
}

EffectorDifference::EffectorDifference(const Effector &e1, const Effector &e2)
    : EffectorDifference(EffectorType::all_world(), e1, e2) {}

EffectorDifference::EffectorDifference(EffectorType type, const Effector &e1,
                                       const Effector &e2) {
  // TODO: Check compatibility
  if (type.location == CoordinateSystem::World && e1.location && e2.location) {
    this->location_ = e1.location->vec - e2.location->vec;
  }
  if (type.rotation == CoordinateSystem::World && e1.rotation && e2.rotation) {
    this->rotation_ =
        e1.rotation->quat * boost::qvm::inverse(e2.rotation->quat);
  }
}

EffectorDifference &EffectorDifference::
operator+=(const EffectorDifference &other) {
  if (this->location_ && other.location_) {
    (*this->location_) += (*other.location_);
  }
  if (this->rotation_ && other.rotation_) {
    // TODO: Don't call normalize here
    boost::qvm::normalize(*this->rotation_);
    (*this->rotation_) *= boost::qvm::normalized(*other.rotation_);
  }
  return *this;
}

EffectorDifference &EffectorDifference::operator*=(std::size_t n) {
  if (this->location_) {
    (*this->location_) *= n;
  }
  if (this->rotation_) {
    if (n == 0) {
      this->rotation_ = boost::qvm::identity_quat<double>();
    } else {
      auto const quat = boost::qvm::normalized(*this->rotation_);
      // TODO: Don't call normalize here
      for (std::size_t i = 0; i < n - 1; i++) {
        boost::qvm::normalize(*this->rotation_);
        (*this->rotation_) *= quat;
      }
    }
  }
  return *this;
}

Effector &Effector::operator+=(const EffectorDifference &other) {
  if (this->location && other.location()) {
    this->location->vec += *other.location();
  }
  if (this->rotation && other.rotation()) {
    // TODO: Don't call normalize here
    boost::qvm::normalize(this->rotation->quat);
    this->rotation->quat *= boost::qvm::normalized(*other.rotation());
  }
  return *this;
}

const std::optional<Location::value_type> &
EffectorDifference::location() const & {
  return this->location_;
}
std::optional<Location::value_type> EffectorDifference::location() && {
  return std::move(this->location_);
}

const std::optional<Rotation::value_type> &
EffectorDifference::rotation() const & {
  return this->rotation_;
}
std::optional<Rotation::value_type> EffectorDifference::rotation() && {
  return std::move(this->rotation_);
}

bool operator==(const EffectorDifference &d1, const EffectorDifference &d2) {
  return d1.location() == d2.location() && d1.rotation() == d2.rotation();
}

bool almost_equal(const EffectorDifference &d1, const EffectorDifference &d2) {
  const bool l =
      static_cast<bool>(d1.location()) == static_cast<bool>(d2.location());
  const bool r =
      static_cast<bool>(d1.rotation()) == static_cast<bool>(d2.rotation());
  if (!l || !r)
    return false;
  bool result = true;
  if (d1.location())
    result = result && almost_equal(*d1.location(), *d2.location());
  if (d1.rotation())
    result = result && almost_equal(*d1.rotation(), *d2.rotation());
  return result;
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

Effector Effector::new_compatible_effector() const {
  Effector e;
  if (this->location) {
    e.location = Location{};
  }
  if (this->rotation) {
    e.rotation = Rotation{};
  }
  return e;
}

bool operator==(const Rotation &r1, const Rotation &r2) {
  return r1.weight == r2.weight && r1.quat == r2.quat;
}

bool operator!=(const Rotation &r1, const Rotation &r2) { return !(r1 == r2); }

bool almost_equal(const Rotation::value_type &q1,
                  const Rotation::value_type &q2) {
  return boost::qvm::cmp(q1, q2,
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
}

bool almost_equal(const Rotation &r1, const Rotation &r2) {
  return almost_equal(r1.weight, r2.weight) && almost_equal(r1.quat, r2.quat);
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

bool almost_equal(const Location::value_type &v1,
                  const Location::value_type &v2) {
  return boost::qvm::cmp(v1, v2,
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
}

bool almost_equal(const Location &l1, const Location &l2) {
  return almost_equal(l1.weight, l2.weight) && almost_equal(l1.vec, l2.vec);
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
