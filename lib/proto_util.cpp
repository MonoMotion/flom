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

#include "flom/proto_util.hpp"
#include "flom/effector.hpp"
#include "flom/motion.hpp"

#include "location.pb.h"
#include "rotation.pb.h"

namespace flom {

namespace proto_util {

Location::value_type unpack_vec3(proto::Vec3 const &vec_proto) {
  return typename Location::value_type(vec_proto.x(), vec_proto.y(),
                                       vec_proto.z());
}

void pack_vec3(Location::value_type const &vec, proto::Vec3 *v_proto) {
  v_proto->set_x(vec.x());
  v_proto->set_y(vec.y());
  v_proto->set_z(vec.z());
}

Location unpack_location(proto::Location const &trans_proto) {
  auto const vec = unpack_vec3(trans_proto.vector());
  return Location{vec};
}

void pack_location(Location const &trans, proto::Location *trans_proto) {
  pack_vec3(trans.vector(), trans_proto->mutable_vector());
}

Rotation::value_type unpack_quat(proto::Quaternion const &quat_proto) {
  return typename Rotation::value_type(quat_proto.w(), quat_proto.x(),
                                       quat_proto.y(), quat_proto.z());
}

void pack_quat(Rotation::value_type const &quat, proto::Quaternion *q_proto) {
  q_proto->set_w(quat.w());
  q_proto->set_x(quat.x());
  q_proto->set_y(quat.y());
  q_proto->set_z(quat.z());
}

Rotation unpack_rotation(proto::Rotation const &rot_proto) {
  auto const quat = unpack_quat(rot_proto.quaternion());
  return Rotation{quat};
}

void pack_rotation(Rotation const &rot, proto::Rotation *rot_proto) {
  pack_quat(rot.quaternion(), rot_proto->mutable_quaternion());
}

proto::EffectorType::Type
pack_coord_system(compat::optional<CoordinateSystem> const &c) {
  if (!c) {
    return proto::EffectorType::Type::EffectorType_Type_None;
  }
  switch (*c) {
  case CoordinateSystem::World:
    return proto::EffectorType::Type::EffectorType_Type_World;
  case CoordinateSystem::Local:
    return proto::EffectorType::Type::EffectorType_Type_Local;
  }
}

void pack_effector_type(EffectorType const &e, proto::EffectorType *proto) {
  proto->set_location(pack_coord_system(e.location()));
  proto->set_rotation(pack_coord_system(e.rotation()));
}

EffectorType unpack_effector_type(proto::EffectorType const &proto) {
  return {unpack_coord_system(proto.location()),
          unpack_coord_system(proto.rotation())};
}

compat::optional<CoordinateSystem>
unpack_coord_system(proto::EffectorType::Type const &proto) {
  if (proto == proto::EffectorType::Type::EffectorType_Type_None) {
    return compat::nullopt;
  } else if (proto == proto::EffectorType::Type::EffectorType_Type_World) {
    return CoordinateSystem::World;
  } else if (proto == proto::EffectorType::Type::EffectorType_Type_Local) {
    return CoordinateSystem::Local;
  } else {
    assert(false); // unreachable
    return CoordinateSystem::World;
  }
}

void pack_effector_weight(EffectorWeight const &weight,
                          proto::EffectorWeight *proto) {
  proto->set_location(weight.location());
  proto->set_rotation(weight.rotation());
}

EffectorWeight unpack_effector_weight(proto::EffectorWeight const &proto) {
  return {proto.location(), proto.rotation()};
}

} // namespace proto_util

} // namespace flom
