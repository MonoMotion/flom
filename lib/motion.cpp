#include "flom/motion.hpp"

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/vec_access.hpp>

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
  Motion m;
  m.model_id = motion_proto.model_id();
  if(motion_proto.loop() == proto::Motion::Loop::Motion_Loop_Wrap) {
    m.loop = LoopType::Wrap;
  } else if(motion_proto.loop() == proto::Motion::Loop::Motion_Loop_None) {
    m.loop = LoopType::None;
  }
  double t_sum = 0;
  for(auto const& frame_proto : motion_proto.frames()) {
    t_sum += frame_proto.duration();
    auto& frame = m.frames[t_sum];
    auto const& changes_proto = frame_proto.changes();
    std::copy(std::cbegin(changes_proto), std::cend(changes_proto), std::inserter(frame.changes, std::end(frame.changes)));
    auto const& effects_proto = frame_proto.effects();
    std::transform(std::cbegin(effects_proto), std::cend(effects_proto), std::inserter(frame.effects, std::end(frame.effects)), [](auto const& p) {
        auto const& effect_proto = p.second;
        Effect e;
        if (effect_proto.has_translation()) {
          e.translation = proto_util::convert_translation(effect_proto.translation().value());
        }
        if (effect_proto.has_rotation()) {
          e.rotation = proto_util::convert_rotation(effect_proto.rotation().value());
        }
        return std::make_pair(p.first, e);
    });
  }

  return std::move(m);
}

void Motion::dump(std::ofstream& f) const {
  auto const m = this->to_protobuf();
  m.SerializeToOstream(&f);
}

proto::Motion Motion::to_protobuf() const {
  proto::Motion m;
  m.set_model_id(this->model_id);
  if(this->loop == LoopType::Wrap) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
  } else if(this->loop == LoopType::None) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_None);
  }
  double last_t;
  for(auto const& [t, frame] : this->frames) {
    auto* frame_proto = m.add_frames();
    frame_proto->set_duration(t - last_t);
    last_t = t;
    // TODO: Use <algorithm> (e.g. std::copy)
    for (auto const& [joint, change] : frame.changes) {
      (*frame_proto->mutable_changes())[joint] = change;
    }
    for (auto const& [link, effect] : frame.effects) {
        proto::Effect e;
        if (effect.translation) {
          proto_util::convert_translation(*effect.translation, e.mutable_translation()->mutable_value());
        }
        if (effect.rotation) {
          proto_util::convert_rotation(*effect.rotation, e.mutable_rotation()->mutable_value());
        }
        (*frame_proto->mutable_effects())[link] = e;
    }
  }

  return std::move(m);
}

namespace proto_util {

boost::qvm::vec<double, 3> convert_vec3(proto::Vec3 const& vec_proto) {
  return boost::qvm::vec<double, 3> { vec_proto.x(), vec_proto.y(), vec_proto.z() };
}

void convert_vec3(boost::qvm::vec<double, 3> const& vec, proto::Vec3* v_proto) {
  v_proto->set_x(boost::qvm::X(vec));
  v_proto->set_y(boost::qvm::Y(vec));
  v_proto->set_z(boost::qvm::Z(vec));
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

void convert_translation(Translation const& trans, proto::Translation* trans_proto) {
  if (trans.coord_system == CoordinateSystem::World) {
    convert_vec3(trans.vec, trans_proto->mutable_world());
  } else if (trans.coord_system == CoordinateSystem::Local) {
     convert_vec3(trans.vec, trans_proto->mutable_local());
  }
  trans_proto->set_weight(trans.weight);
}

boost::qvm::quat<double> convert_quat(proto::Quaternion const& quat_proto) {
  return boost::qvm::quat<double> { quat_proto.w(), quat_proto.x(), quat_proto.y(), quat_proto.z() };
}

void convert_quat(boost::qvm::quat<double> const& quat, proto::Quaternion* q_proto) {
  q_proto->set_w(boost::qvm::S(quat));
  q_proto->set_x(boost::qvm::X(quat));
  q_proto->set_y(boost::qvm::Y(quat));
  q_proto->set_z(boost::qvm::Z(quat));
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

void convert_rotation(Rotation const& rot, proto::Rotation* rot_proto) {
  if (rot.coord_system == CoordinateSystem::World) {
     convert_quat(rot.quat, rot_proto->mutable_world());
  } else if (rot.coord_system == CoordinateSystem::Local) {
     convert_quat(rot.quat, rot_proto->mutable_local());
  }
  rot_proto->set_weight(rot.weight);
}

}

}
