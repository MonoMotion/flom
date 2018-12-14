#include "flom/effector.hpp"
#include "flom/proto_util.hpp"

#include "location.pb.h"
#include "rotation.pb.h"

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace flom {

namespace proto_util {

boost::qvm::vec<double, 3> unpack_vec3(proto::Vec3 const& vec_proto) {
  return boost::qvm::vec<double, 3> { vec_proto.x(), vec_proto.y(), vec_proto.z() };
}

void pack_vec3(boost::qvm::vec<double, 3> const& vec, proto::Vec3* v_proto) {
  v_proto->set_x(boost::qvm::X(vec));
  v_proto->set_y(boost::qvm::Y(vec));
  v_proto->set_z(boost::qvm::Z(vec));
}

Location unpack_location(proto::Location const& trans_proto) {
  Location trans;
  if (trans_proto.has_world()) {
    trans.coord_system = CoordinateSystem::World;
    trans.vec = unpack_vec3(trans_proto.world());
  } else if (trans_proto.has_local()) {
    trans.coord_system = CoordinateSystem::Local;
    trans.vec = unpack_vec3(trans_proto.local());
  }
  trans.weight = trans_proto.weight();
  return std::move(trans);
}

void pack_location(Location const& trans, proto::Location* trans_proto) {
  if (trans.coord_system == CoordinateSystem::World) {
    pack_vec3(trans.vec, trans_proto->mutable_world());
  } else if (trans.coord_system == CoordinateSystem::Local) {
    pack_vec3(trans.vec, trans_proto->mutable_local());
  }
  trans_proto->set_weight(trans.weight);
}

boost::qvm::quat<double> unpack_quat(proto::Quaternion const& quat_proto) {
  return boost::qvm::quat<double> { quat_proto.w(), quat_proto.x(), quat_proto.y(), quat_proto.z() };
}

void pack_quat(boost::qvm::quat<double> const& quat, proto::Quaternion* q_proto) {
  q_proto->set_w(boost::qvm::S(quat));
  q_proto->set_x(boost::qvm::X(quat));
  q_proto->set_y(boost::qvm::Y(quat));
  q_proto->set_z(boost::qvm::Z(quat));
}

Rotation unpack_rotation(proto::Rotation const& rot_proto) {
  Rotation rot;
  if (rot_proto.has_world()) {
    rot.coord_system = CoordinateSystem::World;
    rot.quat = unpack_quat(rot_proto.world());
  } else if (rot_proto.has_local()) {
    rot.coord_system = CoordinateSystem::Local;
    rot.quat = unpack_quat(rot_proto.local());
  }
  rot.weight = rot_proto.weight();
  return std::move(rot);
}

void pack_rotation(Rotation const& rot, proto::Rotation* rot_proto) {
  if (rot.coord_system == CoordinateSystem::World) {
     pack_quat(rot.quat, rot_proto->mutable_world());
  } else if (rot.coord_system == CoordinateSystem::Local) {
     pack_quat(rot.quat, rot_proto->mutable_local());
  }
  rot_proto->set_weight(rot.weight);
}

}

}
