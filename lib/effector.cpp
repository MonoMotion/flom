#include "flom/effect.hpp"
#include "flom/interpolation.hpp"

#include <utility>

#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace flom {

Effector interpolate(double t, Effector const& a, Effector const& b) {
  Effector e;
  if (a.rotation && b.rotation) {
    e.rotation = interpolate(t, *a.rotation, *b.rotation);
  }
  if (a.location && b.location) {
    e.location = interpolate(t, *a.location, *b.location);
  }
  return std::move(e);
}

Rotation interpolate(double t, Rotation const& a, Rotation const& b) {
  Rotation result;
  result.quat = qvm::slerp(a.quat, b.quat, t);
  result.weight = lerp(t, a.weight, b.weight);
  return std::move(result);
}

Location interpolate(double t, Location const& a, Location const& b) {
  Location result;
  result.vec = lerp(t, a.vec, b.vec);
  result.weight = lerp(t, a.weight, b.weight);
  return std::move(result);
}

}


