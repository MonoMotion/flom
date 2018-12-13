#include "flom/motion.hpp"

namespace flom {

Frame Motion::frame_at(double t) const {
  auto const [l, u] = this->frames.equal_range(t);
  if (l->first == t) {
    return l->second; // causes copy...
  } else if (u == this->frames.end()) {
    if (this->loop == LoopType::Wrap) {
      return this->frame_at(t - std::next(l, -1)->first);
    } else {
      throw std::out_of_range("Motion is over");
    }
  } else {
    auto const t1 = std::next(l, -1)->first;
    auto const t2 = u->first;
    auto const& f1 = std::next(l, -1)->second;
    auto const& f2 = u->second;
    return std::move(interpolate((t - t1) / (t2 - t1), f1, f2));
  }
}

Motion Motion::load(std::ifstream& f) {
  proto::Motion m;
  m.ParseFromIstream(&f);
  return std::move(Motion::from_protobuf(m));
}

Motion Motion::from_protobuf(proto::Motion const& motion_proto) {
  throw std::runtime_error("Not implemented");
}

void Motion::dump(std::ofstream& f) const {
  auto const m = this->to_protobuf();
  m.SerializeToOstream(&f);
}

proto::Motion Motion::to_protobuf() const {
  throw std::runtime_error("Not implemented");
}

namespace proto_util {

boost::qvm::vec<double, 3> convert_vec3(proto::Vec3 const& vec_proto) {
  return boost::qvm::vec<double, 3> { vec_proto.x(), vec_proto.y(), vec_proto.z() };
}

Translation convert_translation(proto::Translation const& trans_proto) {
  Translation trans;
  if (trans_proto.has_world()) {
    trans.coord_system = CoordinateSystem::World;
    trans.vec = convert_vec3(trans_proto.world());
  } else if (trans_proto.has_local()) {
    trans.coord_system = CoordinateSystem::Local;
    trans.vec = convert_vec3(trans_proto.local());
  }
  trans.weight = trans_proto.weight();
  return std::move(trans);
}

boost::qvm::quat<double> convert_quat(proto::Quaternion const& quat_proto) {
  return boost::qvm::quat<double> { quat_proto.w(), quat_proto.x(), quat_proto.y(), quat_proto.z() };
}

Rotation convert_rotation(proto::Rotation const& rot_proto) {
  Rotation rot;
  if (rot_proto.has_world()) {
    rot.coord_system = CoordinateSystem::World;
    rot.quat = convert_quat(rot_proto.world());
  } else if (rot_proto.has_local()) {
    rot.coord_system = CoordinateSystem::Local;
    rot.quat = convert_quat(rot_proto.local());
  }
  rot.weight = rot_proto.weight();
  return std::move(rot);
}

}

}
