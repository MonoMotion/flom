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

#ifndef FLOM_PROTO_UTIL_HPP
#define FLOM_PROTO_UTIL_HPP

#include "flom/effector.hpp"
#include "flom/motion.hpp"

#include "frame.pb.h"
#include "location.pb.h"
#include "motion.pb.h"
#include "rotation.pb.h"

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>

#include "flom/compat/optional.hpp"

namespace flom::proto_util {

void pack_vec3(boost::qvm::vec<double, 3> const &, proto::Vec3 *);
void pack_location(Location const &, proto::Location *);

boost::qvm::vec<double, 3> unpack_vec3(proto::Vec3 const &);
Location unpack_location(proto::Location const &);

void pack_quat(boost::qvm::quat<double> const &, proto::Quaternion *);
void pack_rotation(Rotation const &, proto::Rotation *);

boost::qvm::quat<double> unpack_quat(proto::Quaternion const &);
Rotation unpack_rotation(proto::Rotation const &);

void pack_effector_type(EffectorType const &, proto::EffectorType *);
proto::EffectorType::Type
pack_coord_system(compat::optional<CoordinateSystem> const &);

EffectorType unpack_effector_type(proto::EffectorType const &);
compat::optional<CoordinateSystem>
unpack_coord_system(proto::EffectorType::Type const &);

void pack_effector_weight(EffectorWeight const &, proto::EffectorWeight *);
EffectorWeight unpack_effector_weight(proto::EffectorWeight const &);

} // namespace flom::proto_util

#endif
