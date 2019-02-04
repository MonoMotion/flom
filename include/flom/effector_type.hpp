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

#ifndef FLOM_EFFECTOR_TYPE_HPP
#define FLOM_EFFECTOR_TYPE_HPP

#include "flom/effector.hpp"

#include <boost/operators.hpp>

#include "flom/optional.hpp"

namespace flom {

enum class CoordinateSystem { World, Local };

struct EffectorType : boost::operators<EffectorType> {
private:
  optional::optional<CoordinateSystem> location_;
  optional::optional<CoordinateSystem> rotation_;

public:
  EffectorType() = delete;
  EffectorType(optional::optional<CoordinateSystem> location,
               optional::optional<CoordinateSystem> rotation);

  optional::optional<CoordinateSystem> location() const;
  optional::optional<CoordinateSystem> rotation() const;

  void set_location(optional::optional<CoordinateSystem>);
  void clear_location();
  void set_rotation(optional::optional<CoordinateSystem>);
  void clear_rotation();

  Effector new_effector() const;
  bool is_compatible(const Effector &) const;
};

bool operator==(const EffectorType &, const EffectorType &);

} // namespace flom

#endif
