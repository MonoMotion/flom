#ifndef FLOM_INTERPOLATION_HPP
#define FLOM_INTERPOLATION_HPP

#include "flom/effector.hpp"
#include "flom/frame.hpp"

namespace flom {

Location interpolate(double t, Location const& a, Location const& b);
Rotation interpolate(double t, Rotation const& a, Rotation const& b);
Effector interpolate(double t, Effector const& a, Effector const& b);
Frame interpolate(double t, Frame const& a, Frame const& b);

template<typename T>
T lerp(double t, T a, T b) {
  return a + t * (b - a);
}

}

#endif
