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

#include "flom/compat/optional.hpp"

namespace flom {

enum class CoordinateSystem { World, Local };

struct EffectorType : boost::operators<EffectorType> {
private:
  compat::optional<CoordinateSystem> location_;
  compat::optional<CoordinateSystem> rotation_;

public:
  EffectorType() = delete;
  EffectorType(compat::optional<CoordinateSystem> location,
               compat::optional<CoordinateSystem> rotation);

  compat::optional<CoordinateSystem> location() const;
  compat::optional<CoordinateSystem> rotation() const;

  void set_location(compat::optional<CoordinateSystem>);
  void clear_location();
  void set_rotation(compat::optional<CoordinateSystem>);
  void clear_rotation();

  Effector new_effector() const;
  bool is_compatible(const Effector &) const;
};

bool operator==(const EffectorType &, const EffectorType &);

} // namespace flom

#endif
