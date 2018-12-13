#ifndef FLOM_INTERPOLATION_HPP
#define FLOM_INTERPOLATION_HPP

#include "flom/effect.hpp"
#include "flom/frame.hpp"

namespace flom {

Translation interpolate(double t, Translation const& a, Translation const& b);
Rotation interpolate(double t, Rotation const& a, Rotation const& b);
Effect interpolate(double t, Effect const& a, Effect const& b);
Frame interpolate(double t, Frame const& a, Frame const& b);

template<typename T>
T lerp(double t, T a, T b) {
  return a + t * (b - a);
}

}

#endif
