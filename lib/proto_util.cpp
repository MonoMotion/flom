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

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace flom {

namespace proto_util {

boost::qvm::vec<double, 3> unpack_vec3(proto::Vec3 const &vec_proto) {
  return boost::qvm::vec<double, 3>{vec_proto.x(), vec_proto.y(),
                                    vec_proto.z()};
}

void pack_vec3(boost::qvm::vec<double, 3> const &vec, proto::Vec3 *v_proto) {
  v_proto->set_x(boost::qvm::X(vec));
  v_proto->set_y(boost::qvm::Y(vec));
  v_proto->set_z(boost::qvm::Z(vec));
}

Location unpack_location(proto::Location const &trans_proto) {
  Location trans;
  trans.vec = unpack_vec3(trans_proto.vector());
  trans.weight = trans_proto.weight();
  return trans;
}

void pack_location(Location const &trans, proto::Location *trans_proto) {
  pack_vec3(trans.vec, trans_proto->mutable_vector());
  trans_proto->set_weight(trans.weight);
}

boost::qvm::quat<double> unpack_quat(proto::Quaternion const &quat_proto) {
  return boost::qvm::quat<double>{quat_proto.w(), quat_proto.x(),
                                  quat_proto.y(), quat_proto.z()};
}

void pack_quat(boost::qvm::quat<double> const &quat,
               proto::Quaternion *q_proto) {
  q_proto->set_w(boost::qvm::S(quat));
  q_proto->set_x(boost::qvm::X(quat));
  q_proto->set_y(boost::qvm::Y(quat));
  q_proto->set_z(boost::qvm::Z(quat));
}

Rotation unpack_rotation(proto::Rotation const &rot_proto) {
  Rotation rot;
  rot.quat = unpack_quat(rot_proto.quaternion());
  rot.weight = rot_proto.weight();
  return rot;
}

void pack_rotation(Rotation const &rot, proto::Rotation *rot_proto) {
  pack_quat(rot.quat, rot_proto->mutable_quaternion());
  rot_proto->set_weight(rot.weight);
}

proto::EffectorType::Type pack_coord_system(CoordinateSystem const &c) {
  switch (c) {
  case CoordinateSystem::World:
    return proto::EffectorType::Type::EffectorType_Type_World;
  case CoordinateSystem::Local:
    return proto::EffectorType::Type::EffectorType_Type_Local;
  }
}

void pack_effector_type(EffectorType const &e, proto::EffectorType *proto) {
  proto->set_location(pack_coord_system(e.location));
  proto->set_rotation(pack_coord_system(e.rotation));
}

EffectorType unpack_effector_type(proto::EffectorType const &proto) {
  EffectorType e;
  e.location = unpack_coord_system(proto.location());
  e.rotation = unpack_coord_system(proto.rotation());
  return e;
}

CoordinateSystem unpack_coord_system(proto::EffectorType::Type const &proto) {
  if (proto == proto::EffectorType::Type::EffectorType_Type_World) {
    return CoordinateSystem::World;
  } else if (proto == proto::EffectorType::Type::EffectorType_Type_Local) {
    return CoordinateSystem::Local;
  } else {
    assert(false); // unreachable
    return CoordinateSystem::World;
  }
}

} // namespace proto_util

} // namespace flom
