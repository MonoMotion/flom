#include "flom/effector.hpp"
#include "flom/interpolation.hpp"

#include <utility>

#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_traits_array.hpp>

namespace flom {

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
  result.quat = qvm::slerp(a.quat, b.quat, t);
  result.weight = lerp(t, a.weight, b.weight);
  return result;
}

bool operator==(const Rotation &r1, const Rotation &r2) {
  return r1.coord_system == r2.coord_system && r1.weight == r2.weight &&
         r1.quat == r2.quat;
}

bool almost_equal(const Rotation &r1, const Rotation &r2) {
  return r1.coord_system == r2.coord_system && r1.weight == r2.weight &&
         boost::qvm::cmp(r1.quat, r2.quat, [](auto e1, auto e2) {
           return std::abs(e1 - e2) < 0.0001;
         });
}

Location interpolate(double t, Location const &a, Location const &b) {
  Location result;
  result.vec = lerp(t, a.vec, b.vec);
  result.weight = lerp(t, a.weight, b.weight);
  return result;
}

bool operator==(const Location &l1, const Location &l2) {
  return l1.coord_system == l2.coord_system && l1.weight == l2.weight &&
         l1.vec == l2.vec;
}

bool almost_equal(const Location &l1, const Location &l2) {
  return l1.coord_system == l2.coord_system && l1.weight == l2.weight &&
         boost::qvm::cmp(l1.vec, l2.vec, [](auto e1, auto e2) {
           return std::abs(e1 - e2) < 0.0001;
         });
}

Effector &Effector::operator+=(const Effector &x) {
  if (this->location && x.location) {
    this->location->vec += x.location->vec;
  }
  if (this->rotation && x.rotation) {
    this->rotation->quat += x.rotation->quat;
  }
  return *this;
}

Effector &Effector::operator-=(const Effector &x) {
  if (this->location && x.location) {
    this->location->vec -= x.location->vec;
  }
  if (this->rotation && x.rotation) {
    this->rotation->quat -= x.rotation->quat;
  }
  return *this;
}

bool operator==(const Effector &e1, const Effector &e2) {
  return e1.location == e2.location && e1.rotation == e2.rotation;
}

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
    result = result && almost_equal(*e1.location, *e2.location);
  return result;
}

double interpolate(double t, double a, double b) { return lerp(t, a, b); }

} // namespace flom
