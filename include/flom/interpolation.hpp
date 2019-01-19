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

#ifndef FLOM_INTERPOLATION_HPP
#define FLOM_INTERPOLATION_HPP

#include "flom/effector.hpp"
#include "flom/frame.hpp"

namespace flom {

template <typename T, typename U,
          std::enable_if_t<std::is_floating_point<U>::value> * = nullptr>
T lerp(U t, T a, T b) {
  return a + t * (b - a);
}

Location interpolate(double t, Location const &a, Location const &b);
Rotation interpolate(double t, Rotation const &a, Rotation const &b);
Effector interpolate(double t, Effector const &a, Effector const &b);
Frame interpolate(double t, Frame const &a, Frame const &b);
double interpolate(double t, double a, double b);

} // namespace flom

#endif
