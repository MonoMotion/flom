#include "flom/errors.hpp"
#include "flom/motion.hpp"
#include "flom/motion.impl.hpp"
#include "flom/proto_util.hpp"

#include "motion.pb.h"

#include <fstream>
#include <map>
#include <string>

#include <google/protobuf/util/json_util.h>

#include "nlohmann/json.hpp"

namespace flom {

Motion Motion::load(std::ifstream &f) {
  proto::Motion m;
  if (!m.ParseFromIstream(&f)) {
    throw errors::ParseFailedError{};
  }

  return Motion::Impl::from_protobuf(m);
}

Motion Motion::load_json(std::ifstream &f) {
  std::string s;
  f >> s;
  return Motion::load_json_string(s);
}

Motion Motion::load_json_string(std::string const &s) {
  proto::Motion m;
  google::protobuf::util::JsonStringToMessage(s, &m);
  return Motion::Impl::from_protobuf(m);
}

Motion Motion::Impl::from_protobuf(proto::Motion const &motion_proto) {
  Motion m;
  m.impl->model_id = motion_proto.model_id();
  if (motion_proto.loop() == proto::Motion::Loop::Motion_Loop_Wrap) {
    m.impl->loop = LoopType::Wrap;
  } else if (motion_proto.loop() == proto::Motion::Loop::Motion_Loop_None) {
    m.impl->loop = LoopType::None;
  }
  for (auto const &frame_proto : motion_proto.frames()) {
    auto &frame = m.impl->raw_frames[frame_proto.t()];
    auto const &positions_proto = frame_proto.positions();
    std::copy(std::cbegin(positions_proto), std::cend(positions_proto),
              std::inserter(frame.positions, std::end(frame.positions)));
    auto const &effectors_proto = frame_proto.effectors();
    std::transform(std::cbegin(effectors_proto), std::cend(effectors_proto),
                   std::inserter(frame.effectors, std::end(frame.effectors)),
                   [](auto const &p) {
                     auto const &effect_proto = p.second;
                     Effector e;
                     if (effect_proto.has_location()) {
                       e.location = proto_util::unpack_location(
                           effect_proto.location().value());
                     }
                     if (effect_proto.has_rotation()) {
                       e.rotation = proto_util::unpack_rotation(
                           effect_proto.rotation().value());
                     }
                     // TODO: Delete copy
                     return std::make_pair(p.first, e);
                   });
  }

  // copy occurs...
  return m;
}
void Motion::dump(std::ofstream &f) const {
  auto const m = this->impl->to_protobuf();
  if (!m.SerializeToOstream(&f)) {
    throw errors::SerializationFailedError{};
  }
}

void Motion::dump_json(std::ofstream &f) const {
  f << this->dump_json_string();
}

std::string Motion::dump_json_string() const {
  std::string s;
  auto const m = this->impl->to_protobuf();
  google::protobuf::util::JsonPrintOptions opt;
  opt.always_print_primitive_fields = true;
  google::protobuf::util::MessageToJsonString(m, &s, opt);
  return s;
}

proto::Motion Motion::Impl::to_protobuf() const {
  proto::Motion m;
  m.set_model_id(this->model_id);
  if (this->loop == LoopType::Wrap) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
  } else if (this->loop == LoopType::None) {
    m.set_loop(proto::Motion::Loop::Motion_Loop_None);
  }
  for (auto const &[t, frame] : this->raw_frames) {
    auto *frame_proto = m.add_frames();
    frame_proto->set_t(t);
    // TODO: Use <algorithm> (e.g. std::copy)
    for (auto const &[joint, change] : frame.positions) {
      (*frame_proto->mutable_positions())[joint] = change;
    }
    for (auto const &[link, effect] : frame.effectors) {
      proto::Effector e;
      if (effect.location) {
        proto_util::pack_location(*effect.location,
                                  e.mutable_location()->mutable_value());
      }
      if (effect.rotation) {
        proto_util::pack_rotation(*effect.rotation,
                                  e.mutable_rotation()->mutable_value());
      }
      (*frame_proto->mutable_effectors())[link] = e;
    }
  }

  return m;
}

Motion Motion::load_legacy_json(std::ifstream &s) {
  using json = nlohmann::json;

  json json_data;
  s >> json_data;

  Motion m;
  m.impl->model_id = json_data["model"];
  {
    auto loop_type = json_data["loop"];
    if (loop_type == "wrap") {
      m.impl->loop = LoopType::Wrap;
    } else if (loop_type == "none") {
      m.impl->loop = LoopType::None;
    } else {
      throw std::runtime_error("Unknown loop type");
    }
  }
  {
    auto const frames = json_data["frames"];
    for (auto const &frame : frames) {
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
          auto const &value = trans_data["value"];
          if (trans_data["space"] == "world") {
            trans.coord_system = CoordinateSystem::World;
          } else if (trans_data["space"] == "local") {
            trans.coord_system = CoordinateSystem::Local;
          }
          trans.vec = boost::qvm::vec<double, 3>{value[0], value[1], value[2]};
          trans.weight = trans_data["weight"];
          e.location = std::move(trans);
        }

        if (effect_data.count("rotation") != 0) {
          Rotation rot;
          auto const rot_data = effect_data["rotation"];
          auto const &value = rot_data["value"];
          if (rot_data["space"] == "world") {
            rot.coord_system = CoordinateSystem::World;
          } else if (rot_data["space"] == "local") {
            rot.coord_system = CoordinateSystem::Local;
          }
          rot.quat =
              boost::qvm::quat<double>{value[0], value[1], value[2], value[3]};
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

} // namespace flom
