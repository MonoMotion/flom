#include "flom/motion.hpp"
#include "flom/interpolation.hpp"
#include "flom/proto_util.hpp"
#include "flom/range.hpp"

#include "motion.pb.h"

#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cassert>

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <google/protobuf/util/json_util.h>

#include "nlohmann/json.hpp"

namespace flom {

class Motion::Impl {
public:
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> raw_frames;

  Impl()
    : model_id(), loop(LoopType::None), raw_frames() {}
  Impl(std::string const& model)
    : model_id(model), loop(LoopType::None), raw_frames() {}

  static Motion from_protobuf(proto::Motion const&);
  proto::Motion to_protobuf() const;
};

Motion::Motion() : impl(std::make_unique<Motion::Impl>()) {}
Motion::Motion(std::string const& model) : impl(std::make_unique<Motion::Impl>(model)) {}
Motion::Motion(Motion const& m) : impl(std::make_unique<Motion::Impl>(*m.impl)) {}
Motion::~Motion() {}

Frame Motion::frame_at(double t) const {
  if (t < 0) {
    throw std::out_of_range("t must be positive");
  }
  auto const [l, u] = this->impl->raw_frames.equal_range(t);
  if (l->first == t) {
    // found a frame with exactly same time
    return l->second; // causes copy...
  } else if (u == this->impl->raw_frames.end()) {
    // Out of frames
    if (this->impl->loop == LoopType::Wrap) {
      auto const& last = std::next(l, -1);
      auto const motion_length = last->first;
      unsigned const skip_episode = t / motion_length;
      auto const trailing_t = t - skip_episode * motion_length;
      return this->frame_at(trailing_t) + last->second * skip_episode;
    } else {
      throw std::out_of_range("Motion is over");
    }
  } else {
    // Between two frames -> interpolate
    auto const t1 = std::next(l, -1)->first;
    auto const t2 = u->first;
    auto const& f1 = std::next(l, -1)->second;
    auto const& f2 = u->second;
    return std::move(interpolate((t - t1) / (t2 - t1), f1, f2));
  }
}

FrameRange Motion::frames(double fps) const {
  return FrameRange{*this, fps};
}

bool Motion::is_in_range_at(double t) const {
  if (this->impl->loop == LoopType::Wrap) {
    return true;
  } else {
    auto const last_t = std::next(this->impl->raw_frames.end(), -1)->first;
    return t <= last_t;
  }
}

Motion Motion::load(std::ifstream& f) {
  proto::Motion m;
  m.ParseFromIstream(&f);
  return Motion::Impl::from_protobuf(m);
}

Motion Motion::load_json(std::ifstream& f) {
  std::string s;
  f >> s;
  return Motion::load_json_string(s);
}

Motion Motion::load_json_string(std::string const& s) {
  proto::Motion m;
  google::protobuf::util::JsonStringToMessage(s, &m);
  return Motion::Impl::from_protobuf(m);
}

Motion Motion::Impl::from_protobuf(proto::Motion const& motion_proto) {
  Motion m;
  m.impl->model_id = motion_proto.model_id();
  if(motion_proto.loop() == proto::Motion::Loop::Motion_Loop_Wrap) {
    m.impl->loop = LoopType::Wrap;
  } else if(motion_proto.loop() == proto::Motion::Loop::Motion_Loop_None) {
    m.impl->loop = LoopType::None;
  }
  double t_sum = 0;
  for(auto const& frame_proto : motion_proto.frames()) {
    t_sum += frame_proto.t();
    auto& frame = m.impl->raw_frames[t_sum];
    auto const& positions_proto = frame_proto.positions();
    std::copy(std::cbegin(positions_proto), std::cend(positions_proto), std::inserter(frame.positions, std::end(frame.positions)));
    auto const& effectors_proto = frame_proto.effectors();
    std::transform(std::cbegin(effectors_proto), std::cend(effectors_proto), std::inserter(frame.effectors, std::end(frame.effectors)), [](auto const& p) {
        auto const& effect_proto = p.second;
        Effector e;
        if (effect_proto.has_location()) {
          e.location = proto_util::unpack_location(effect_proto.location().value());
        }
        if (effect_proto.has_rotation()) {
          e.rotation = proto_util::unpack_rotation(effect_proto.rotation().value());
        }
        // TODO: Delete copy
        return std::make_pair(p.first, e);
    });
  }

  // copy occurs...
  return m;
}

Frame& Motion::get_or_insert_frame(double t) {
  return this->impl->raw_frames[t];
}

void Motion::dump(std::ofstream& f) const {
  auto const m = this->impl->to_protobuf();
  m.SerializeToOstream(&f);
}

void Motion::dump_json(std::ofstream& f) const {
  f << this->dump_json_string();
}

std::string Motion::dump_json_string() const {
  std::string s;
  auto const m = this->impl->to_protobuf();
  google::protobuf::util::JsonPrintOptions opt;
  opt.always_print_primitive_fields = true;
  google::protobuf::util::MessageToJsonString(m, &s, opt);
  return std::move(s);
}

proto::Motion Motion::Impl::to_protobuf() const {
  proto::Motion m;
  m.set_model_id(this->model_id);
  if(this->loop == LoopType::Wrap) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
  } else if(this->loop == LoopType::None) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_None);
  }
  for(auto const& [t, frame] : this->raw_frames) {
    auto* frame_proto = m.add_frames();
    frame_proto->set_t(t);
    // TODO: Use <algorithm> (e.g. std::copy)
    for (auto const& [joint, change] : frame.positions) {
      (*frame_proto->mutable_positions())[joint] = change;
    }
    for (auto const& [link, effect] : frame.effectors) {
        proto::Effector e;
        if (effect.location) {
          proto_util::pack_location(*effect.location, e.mutable_location()->mutable_value());
        }
        if (effect.rotation) {
          proto_util::pack_rotation(*effect.rotation, e.mutable_rotation()->mutable_value());
        }
        (*frame_proto->mutable_effectors())[link] = e;
    }
  }

  return std::move(m);
}

Motion Motion::load_legacy_json(std::ifstream &s) {
  using json = nlohmann::json;

  json json_data;
  s >> json_data;

  Motion m;
  m.impl->model_id = json_data["model"];
  {
    auto loop_type = json_data["loop"];
    if(loop_type == "wrap") {
      m.impl->loop = LoopType::Wrap;
    } else if(loop_type == "none") {
      m.impl->loop = LoopType::None;
    } else {
      throw std::runtime_error("Unknown loop type");
    }
  }
  {
    auto const frames = json_data["frames"];
    for(auto const& frame : frames) {
      Frame f;
      const double time = frame["timepoint"];
      auto const positions = frame["position"];
      // TODO: Use <algorithm> (e.g. std::copy)
      for (auto it = std::cbegin(positions); it != std::cend(positions); ++it) {
        f.positions[it.key()] = it.value();
      }
      auto const effectors = frame["effector"];
      for (auto it = std::cbegin(effectors); it != std::cend(effectors); ++it) {
        Effector e;
        auto const effect_data = it.value();
        if (effect_data.count("location") != 0) {
          Location trans;
          auto const trans_data = effect_data["location"];
          auto const& value = trans_data["value"];
          if (trans_data["space"] == "world") {
            trans.coord_system = CoordinateSystem::World;
          } else if (trans_data["space"] == "local") {
            trans.coord_system = CoordinateSystem::Local;
          }
          trans.vec = boost::qvm::vec<double, 3>{ value[0], value[1], value[2] };
          trans.weight = trans_data["weight"];
          e.location = std::move(trans);
        }

        if (effect_data.count("rotation") != 0) {
          Rotation rot;
          auto const rot_data = effect_data["rotation"];
          auto const& value = rot_data["value"];
          if (rot_data["space"] == "world") {
            rot.coord_system = CoordinateSystem::World;
          } else if (rot_data["space"] == "local") {
            rot.coord_system = CoordinateSystem::Local;
          }
          rot.quat = boost::qvm::quat<double>{ value[0], value[1], value[2], value[3] };
          rot.weight = rot_data["weight"];
          e.rotation = std::move(rot);
        }

        f.effectors[it.key()] = e;
      }
      m.impl->raw_frames[time] = std::move(f);
    }
  }
  // copy occurs...
  return m;
}

LoopType Motion::loop() const {
  return this->impl->loop;
}

void Motion::set_loop(LoopType loop) {
  this->impl->loop = loop;
}

std::string Motion::model_id() const {
  return this->impl->model_id;
}

void Motion::set_model_id(std::string const& model_id) {
  this->impl->model_id = model_id;
}


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
