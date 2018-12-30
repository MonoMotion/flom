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

#ifndef FLOM_MOTION_IMPL_HPP
#define FLOM_MOTION_IMPL_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include "motion.pb.h"

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace flom {

class Motion::Impl {
public:
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> raw_frames;
  std::unordered_set<std::string> joint_names;
  std::unordered_map<std::string, EffectorType> effector_types;

  Impl() : loop(LoopType::None) { this->add_initial_frame(); }

  Impl(const std::unordered_set<std::string> &joints,
       const std::unordered_map<std::string, EffectorType> &effectors,
       const std::string &model = "")
      : model_id(model), loop(LoopType::None), raw_frames(),
        joint_names(joints), effector_types(effectors) {
    this->add_initial_frame();
  }

  Impl(const std::unordered_set<std::string> &joints,
       const std::unordered_set<std::string> &effectors,
       const std::string &model = "")
      : model_id(model), loop(LoopType::None), raw_frames(),
        joint_names(joints), effector_types() {
    this->effector_types.reserve(effectors.size());
    for (const auto &name : effectors) {
      this->effector_types.emplace(name, EffectorType{});
    }
    this->add_initial_frame();
  }

  void add_initial_frame();

  static Motion from_protobuf(proto::Motion const &);
  proto::Motion to_protobuf() const;

  bool is_valid() const;
};

} // namespace flom

#endif
