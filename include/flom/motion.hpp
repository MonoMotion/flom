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

#ifndef FLOM_MOTION_HPP
#define FLOM_MOTION_HPP

#include "flom/effector_type.hpp"
#include "flom/frame.hpp"

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace flom {

enum class LoopType { None, Wrap };

class FrameRange;
class KeyframeRange;

class Motion {
  friend bool operator==(const Motion &, const Motion &);

public:
  static Motion load(std::ifstream &);
  static Motion load_json(std::ifstream &);
  static Motion load_json_string(std::string const &);
  static Motion load_legacy_json(std::ifstream &);

  Motion(const std::unordered_set<std::string> &joint_names,
         const std::unordered_set<std::string> &effector_names,
         const std::string &model = "");

  Motion(const std::unordered_set<std::string> &joint_names,
         const std::unordered_map<std::string, EffectorType> &effector_types,
         const std::string &model = "");

  Motion(Motion const &);
  ~Motion();

  bool is_valid() const;
  bool is_valid_frame(const Frame &) const;

  Frame frame_at(double t) const;

  FrameRange frames(double fps) const;

  bool is_in_range_at(double t) const;

  void dump(std::ofstream &) const;
  void dump_json(std::ofstream &) const;
  std::string dump_json_string() const;

  LoopType loop() const;
  void set_loop(LoopType);

  std::string model_id() const;
  void set_model_id(std::string const &);

  Frame new_keyframe() const;
  void insert_keyframe(double t, const Frame &);
  void delete_keyframe(double t, bool loose = true);
  KeyframeRange keyframes();

  EffectorType effector_type(const std::string &) const;
  void set_effector_type(const std::string &, EffectorType);

  double length() const;

  KeyRange<std::string> joint_names() const;
  KeyRange<std::string> effector_names() const;

  FrameDifference world_effector_difference(const Frame &, const Frame &) const;

private:
  class Impl;
  std::unique_ptr<Impl> impl;
};

bool operator==(const Motion &, const Motion &);

} // namespace flom

#endif
