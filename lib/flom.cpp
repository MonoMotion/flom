#include <fstream>
#include <stdexcept>

#include "flom/flom.hpp"

#include "motion.pb.h"
#include "frame.pb.h"
#include "translation.pb.h"
#include "rotation.pb.h"

#include "nlohmann/json.hpp"

namespace flom {

using json = nlohmann::json;

proto::Motion import_legacy_json(std::ifstream &f) {
  json json_data;
  f >> json_data;

  proto::Motion m;
  m.set_model_id(json_data["model"]);
  {
    auto loop_type = json_data["loop"];
    if(loop_type == "wrap") {
      m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
    } else if(loop_type == "none") {
      m.set_loop(proto::Motion::Loop::Motion_Loop_None);
    } else {
      throw std::runtime_error("Unknown loop type");
    }
  }
  {
    auto const frames = json_data["frames"];
    for (auto const& frame : frames) {
      auto* new_frame = m.add_frames();
      new_frame->set_duration(frame["timepoint"]);
      auto const positions = frame["position"];
      for (auto it = std::cbegin(positions); it != std::cend(positions); ++it) {
        (*new_frame->mutable_changes())[it.key()] = it.value();
      }
      auto const effectors = frame["effector"];
      for (auto it = std::cbegin(effectors); it != std::cend(effectors); ++it) {
        proto::Effect e;
        auto const effect_data = it.value();
        if (effect_data.count("location") != 0) {
          auto* trans_val = e.mutable_translation();
          auto* trans = trans_val->mutable_value();
          proto::Vec3* vec_ptr;
          auto const trans_data = effect_data["location"];
          if (trans_data["space"] == "world") {
            vec_ptr = trans->mutable_world();
          } else if (trans_data["space"] == "local") {
            vec_ptr = trans->mutable_local();
          }
          vec_ptr->set_x(trans_data["value"][0]);
          vec_ptr->set_y(trans_data["value"][1]);
          vec_ptr->set_z(trans_data["value"][2]);

          trans->set_weight(trans_data["weight"]);
        }

        if (effect_data.count("rotation") != 0) {
          auto* rot_val = e.mutable_rotation();
          auto* rot = rot_val->mutable_value();
          proto::Quaternion* q_ptr;
          auto const rot_data = effect_data["rotation"];
          if (rot_data["space"] == "world") {
            q_ptr = rot->mutable_world();
          } else if (rot_data["space"] == "local") {
            q_ptr = rot->mutable_local();
          }
          q_ptr->set_w(rot_data["value"][0]);
          q_ptr->set_x(rot_data["value"][1]);
          q_ptr->set_y(rot_data["value"][2]);
          q_ptr->set_z(rot_data["value"][3]);

          rot->set_weight(rot_data["weight"]);
        }

        (*new_frame->mutable_effects())[it.key()] = e;
      }
    }
  }
  return m;
}

}
