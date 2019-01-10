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

Location::Location() = default;
Location::Location(const Location::value_type &vector) : vector_(vector) {}

const Location::value_type &Location::vector() const { return this->vector_; }

void Location::set_vector(const Location::value_type &vector) {
  this->vector_ = vector;
}

Location &Location::operator+=(const Location &other) {
  this->vector_ += other.vector_;
  return *this;
}

Location &Location::operator-=(const Location &other) {
  this->vector_ -= other.vector_;
  return *this;
}

Location &Location::operator*=(std::size_t n) {
  this->vector_ *= n;
  return *this;
}

bool operator==(const Location &l1, const Location &l2) {
  return l1.vector() == l2.vector();
}

bool almost_equal(const Location &l1, const Location &l2) {
  return boost::qvm::cmp(l1.vector(), l2.vector(),
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
}

Rotation::Rotation() : quat_({1, 0, 0, 0}) {}
Rotation::Rotation(const Rotation::value_type &quat)
    : quat_(boost::qvm::normalized(quat)) {}

const Rotation::value_type &Rotation::quaternion() const { return this->quat_; }

void Rotation::set_quaternion(const Rotation::value_type &quat) {
  this->quat_ = boost::qvm::normalized(quat);
}

Rotation &Rotation::operator+=(const Rotation &other) {
  this->set_quaternion(this->quat_ * other.quat_);
  return *this;
}

Rotation &Rotation::operator-=(const Rotation &other) {
  this->set_quaternion(this->quat_ * boost::qvm::conjugate(other.quat_));
  return *this;
}

Rotation &Rotation::operator*=(std::size_t n) {
  if (n == 0) {
    this->set_quaternion(boost::qvm::identity_quat<double>());
  } else {
    auto const quat = this->quat_;
    for (std::size_t i = 0; i < n - 1; i++) {
      this->set_quaternion(this->quat_ * quat);
    }
  }
  return *this;
}

bool operator==(const Rotation &r1, const Rotation &r2) {
  return r1.quaternion() == r2.quaternion();
}

bool almost_equal(const Rotation &r1, const Rotation &r2) {
  return boost::qvm::cmp(r1.quaternion(), r2.quaternion(),
                         [](auto e1, auto e2) { return almost_equal(e1, e2); });
  ;
}
EffectorDifference operator-(const Effector &e1, const Effector &e2) {
  return EffectorDifference{e1, e2};
}

EffectorDifference::EffectorDifference(const Effector &e1, const Effector &e2) {
  // Not throwing exception in favor of better performance
  assert(e1.is_compatible(e2) &&
         "Cannot perform the operation on Incompatible effectors");

  if (e1.location() && e2.location()) {
    this->location_ = *e1.location() - *e2.location();
  }
  if (e1.rotation() && e2.rotation()) {
    this->rotation_ = *e1.rotation() - *e2.rotation();
  }
}

EffectorDifference &EffectorDifference::
operator+=(const EffectorDifference &other) {
  // Not throwing exception in favor of better performance
  assert(this->is_compatible(other) &&
         "Cannot use incompatibe FrameDifference instance");

  if (this->location_ && other.location_) {
    this->location_.value() += *other.location_;
  }
  if (this->rotation_ && other.rotation_) {
    this->rotation_.value() += *other.rotation_;
  }
  return *this;
}

EffectorDifference &EffectorDifference::operator*=(std::size_t n) {
  if (this->location_) {
    this->location_.value() *= n;
  }
  if (this->rotation_) {
    this->rotation_.value() *= n;
  }
  return *this;
}

Effector &Effector::operator+=(const EffectorDifference &other) {
  // Not throwing exception in favor of better performance
  assert(this->is_compatible(other) &&
         "Cannot use incompatibe FrameDifference instance");

  if (this->location() && other.location()) {
    this->set_location(*this->location() + *other.location());
  }
  if (this->rotation() && other.rotation()) {
    this->set_rotation(*this->rotation() + *other.rotation());
  }
  return *this;
}

const std::optional<Location> &EffectorDifference::location() const & {
  return this->location_;
}
std::optional<Location> EffectorDifference::location() && {
  return std::move(this->location_);
}

const std::optional<Rotation> &EffectorDifference::rotation() const & {
  return this->rotation_;
}
std::optional<Rotation> EffectorDifference::rotation() && {
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
  // Not throwing exception in favor of better performance
  assert(a.is_compatible(b) &&
         "Cannot perform the operation on Incompatible effectors");

  Effector e;
  if (a.rotation() && b.rotation()) {
    e.set_rotation(interpolate(t, *a.rotation(), *b.rotation()));
  }
  if (a.location() && b.location()) {
    e.set_location(interpolate(t, *a.location(), *b.location()));
  }
  return e;
}

Effector::Effector() : location_(std::nullopt), rotation_(std::nullopt) {}
Effector::Effector(const std::optional<Location> &location,
                   const std::optional<Rotation> &rotation)
    : location_(location), rotation_(rotation) {}

const std::optional<Location> &Effector::location() const & {
  return this->location_;
}
std::optional<Location> Effector::location() && {
  return std::move(this->location_);
}

void Effector::set_location(const std::optional<Location> &location) {
  this->location_ = location;
}

void Effector::clear_location() { this->set_location(std::nullopt); }

const std::optional<Rotation> &Effector::rotation() const & {
  return this->rotation_;
}
std::optional<Rotation> Effector::rotation() && {
  return std::move(this->rotation_);
}

void Effector::set_rotation(const std::optional<Rotation> &rotation) {
  this->rotation_ = rotation;
}

void Effector::clear_rotation() { this->set_rotation(std::nullopt); }

Effector Effector::new_compatible_effector() const {
  Effector e;
  if (this->location()) {
    e.set_location(Location{});
  }
  if (this->rotation()) {
    e.set_rotation(Rotation{});
  }
  return e;
}

bool EffectorDifference::is_compatible(const EffectorDifference &other) const {
  bool loc_v = static_cast<bool>(other.location());
  bool loc_t = static_cast<bool>(this->location());
  bool rot_v = static_cast<bool>(other.rotation());
  bool rot_t = static_cast<bool>(this->rotation());
  return loc_v == loc_t && rot_v == rot_t;
}

bool Effector::is_compatible(const EffectorDifference &other) const {
  bool loc_v = static_cast<bool>(other.location());
  bool loc_t = static_cast<bool>(this->location());
  bool rot_v = static_cast<bool>(other.rotation());
  bool rot_t = static_cast<bool>(this->rotation());
  return loc_v == loc_t && rot_v == rot_t;
}

bool Effector::is_compatible(const Effector &other) const {
  bool loc_v = static_cast<bool>(other.location());
  bool loc_t = static_cast<bool>(this->location());
  bool rot_v = static_cast<bool>(other.rotation());
  bool rot_t = static_cast<bool>(this->rotation());
  return loc_v == loc_t && rot_v == rot_t;
}

Location interpolate(double t, Location const &a, Location const &b) {
  auto const vec = lerp(t, a.vector(), b.vector());
  return Location{vec};
}

Rotation interpolate(double t, Rotation const &a, Rotation const &b) {
  // TODO: Check possibility of NaNs before calling qvm::slerp
  auto quat = qvm::slerp(a.quaternion(), b.quaternion(), t);
  if (std::isnan(qvm::S(quat))) {
    quat = a.quaternion();
  }

  return Rotation{quat};
}

bool operator==(const Effector &e1, const Effector &e2) {
  return e1.location() == e2.location() && e1.rotation() == e2.rotation();
}

bool operator!=(const Effector &e1, const Effector &e2) { return !(e1 == e2); }

bool almost_equal(const Effector &e1, const Effector &e2) {
  // TODO: Refactor: Remove mutable variable
  const bool l =
      static_cast<bool>(e1.location()) == static_cast<bool>(e2.location());
  const bool r =
      static_cast<bool>(e1.rotation()) == static_cast<bool>(e2.rotation());
  if (!l || !r)
    return false;
  bool result = true;
  if (e1.location())
    result = result && almost_equal(*e1.location(), *e2.location());
  if (e1.rotation())
    result = result && almost_equal(*e1.rotation(), *e2.rotation());
  return result;
}

double interpolate(double t, double a, double b) { return lerp(t, a, b); }
bool almost_equal(double a, double b) {
  return boost::math::fpc::close_at_tolerance<double>(
      constants::float_point_tolerance)(a, b);
}

} // namespace flom
