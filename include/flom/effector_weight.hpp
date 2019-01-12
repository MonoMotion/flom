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

#ifndef FLOM_EFFECTOR_WEIGHT_HPP
#define FLOM_EFFECTOR_WEIGHT_HPP

namespace flom {

class EffectorWeight {
private:
  double location_;
  double rotation_;

  static double validate_weight(double);

public:
  EffectorWeight() = delete;

  EffectorWeight(double location, double rotation);

  double location() const noexcept;
  double rotation() const noexcept;

  void set_location(double);
  void set_rotation(double);
};

} // namespace flom

#endif
