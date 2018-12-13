#include "flom/motion.hpp"
#include "flom/interpolation.hpp"
#include "flom/proto_util.hpp"

#include <cassert>

#include <boost/qvm/quat_access.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <google/protobuf/util/json_util.h>

#include "nlohmann/json.hpp"

namespace flom {

Frame Motion::frame_at(double t) const {
  auto const [l, u] = this->frames.equal_range(t);
  if (l->first == t) {
    // found a frame with exactly same time
    return l->second; // causes copy...
  } else if (u == this->frames.end()) {
    // Out of frames
    if (this->loop == LoopType::Wrap) {
      return this->frame_at(t - std::next(l, -1)->first);
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

Motion Motion::load(std::ifstream& f) {
  proto::Motion m;
  m.ParseFromIstream(&f);
  return std::move(Motion::from_protobuf(m));
}

Motion Motion::load_json(std::ifstream& f) {
  proto::Motion m;
  std::string s;
  f >> s;
  google::protobuf::util::JsonStringToMessage(s, &m);
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
  auto const& initial_positions_proto = motion_proto.initial_positions();
  std::copy(std::cbegin(initial_positions_proto), std::cend(initial_positions_proto), std::inserter(m.initial_positions, std::end(m.initial_positions)));
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
          e.translation = proto_util::unpack_translation(effect_proto.translation().value());
        }
        if (effect_proto.has_rotation()) {
          e.rotation = proto_util::unpack_rotation(effect_proto.rotation().value());
        }
        // TODO: Delete copy
        return std::make_pair(p.first, e);
    });
  }

  return std::move(m);
}

void Motion::dump(std::ofstream& f) const {
  auto const m = this->to_protobuf();
  m.SerializeToOstream(&f);
}

void Motion::dump_json(std::ofstream& f) const {
  std::string s;
  auto const m = this->to_protobuf();
  google::protobuf::util::MessageToJsonString(m, &s);
  f << s;
}

proto::Motion Motion::to_protobuf() const {
  proto::Motion m;
  m.set_model_id(this->model_id);
  if(this->loop == LoopType::Wrap) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
  } else if(this->loop == LoopType::None) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_None);
  }
  for (auto const& [joint, position] : this->initial_positions) {
    (*m.mutable_initial_positions())[joint] = position;
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
          proto_util::pack_translation(*effect.translation, e.mutable_translation()->mutable_value());
        }
        if (effect.rotation) {
          proto_util::pack_rotation(*effect.rotation, e.mutable_rotation()->mutable_value());
        }
        (*frame_proto->mutable_effects())[link] = e;
    }
  }

  return std::move(m);
}

Motion Motion::load_legacy_json(std::ifstream &s) {
  using json = nlohmann::json;

  json json_data;
  s >> json_data;

  Motion m;
  m.model_id = json_data["model"];
  {
    auto loop_type = json_data["loop"];
    if(loop_type == "wrap") {
      m.loop = LoopType::Wrap;
    } else if(loop_type == "none") {
      m.loop = LoopType::None;
    } else {
      throw std::runtime_error("Unknown loop type");
    }
  }
  {
    auto const frames = json_data["frames"];
    {
      auto const initial_positions = frames[0]["position"];
      for (auto it = std::cbegin(initial_positions); it != std::cend(initial_positions); ++it) {
        m.initial_positions[it.key()] = it.value();
      }
    }
    std::unordered_map<std::string, double> last_positions (std::cbegin(m.initial_positions), std::cend(m.initial_positions));
    std::unordered_map<std::string, Effect> last_effects;
    for(auto const& frame : frames) {
      Frame f;
      const double duration = frame["timepoint"];
      auto const positions = frame["position"];
      // TODO: Use <algorithm> (e.g. std::copy)
      for (auto it = std::cbegin(positions); it != std::cend(positions); ++it) {
        f.changes[it.key()] = static_cast<double>(it.value()) - last_positions.at(it.key());
        last_positions[it.key()] = it.value();
      }
      auto const effectors = frame["effector"];
      for (auto it = std::cbegin(effectors); it != std::cend(effectors); ++it) {
        Effect e;
        auto const effect_data = it.value();
        if (effect_data.count("location") != 0) {
          Translation trans;
          auto const trans_data = effect_data["location"];
          auto const& value = trans_data["value"];
          if (trans_data["space"] == "world") {
            trans.coord_system = CoordinateSystem::World;
          } else if (trans_data["space"] == "local") {
            trans.coord_system = CoordinateSystem::Local;
          }
          trans.vec = boost::qvm::vec<double, 3>{ value[0], value[1], value[2] };
          trans.weight = trans_data["weight"];
          e.translation = std::move(trans);
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

        if (last_effects.count(it.key()) != 0) {
          // Don't define operator- for Effect,
          // Because that would only be used here
          auto const& last = last_effects.at(it.key());
          auto& new_effect = f.effects[it.key()];
          assert(static_cast<bool>(last.translation) == static_cast<bool>(e.translation));
          assert(static_cast<bool>(last.rotation) == static_cast<bool>(e.rotation));
          if (last.translation && e.translation) {
            assert(last.translation->weight == e.translation->weight);
            assert(last.translation->coord_system == e.translation->coord_system);
            Translation trans;
            trans.weight = e.translation->weight;
            trans.coord_system = e.translation->coord_system;
            trans.vec = e.translation->vec - last.translation->vec;
            new_effect.translation = trans;
          }
          if (last.rotation && e.rotation) {
            assert(last.rotation->weight == e.rotation->weight);
            assert(last.rotation->coord_system == e.rotation->coord_system);
            Rotation rot;
            rot.weight = e.rotation->weight;
            rot.coord_system = e.rotation->coord_system;
            rot.quat = e.rotation->quat - last.rotation->quat;
            new_effect.rotation = rot;
          }
        } else {
          f.effects[it.key()] = e;
        }

        last_effects[it.key()] = e;
      }
      m.frames[duration] = std::move(f);
    }
  }
  return m;
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

Translation unpack_translation(proto::Translation const& trans_proto) {
  Translation trans;
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

void pack_translation(Translation const& trans, proto::Translation* trans_proto) {
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
