#include "flom/motion.hpp"
#include "flom/errors.hpp"
#include "flom/interpolation.hpp"
#include "flom/motion.impl.hpp"
#include "flom/range.hpp"

#include "motion.pb.h"

#include <cmath>
#include <string>

namespace flom {

Motion::Motion() : impl(std::make_unique<Motion::Impl>()) {}
Motion::Motion(std::string const &model)
    : impl(std::make_unique<Motion::Impl>(model)) {}
Motion::Motion(Motion const &m)
    : impl(std::make_unique<Motion::Impl>(*m.impl)) {}
Motion::~Motion() {}

bool Motion::is_valid() const { return this->impl && this->impl->is_valid(); }

Frame Motion::frame_at(double t) const {
  if (std::isnan(t) || t < 0) {
    throw errors::InvalidTimeError(t);
  }
  auto const [l, u] = this->impl->raw_frames.equal_range(t);
  if (l->first == t) {
    // found a frame with exactly same time
    return l->second; // causes copy...
  } else if (u == this->impl->raw_frames.end()) {
    // Out of frames
    if (this->impl->loop == LoopType::Wrap) {
      auto const &last = std::next(l, -1);
      auto const motion_length = last->first;
      if (motion_length == 0) {
        // only one frame with t == 0
        return last->second;
      }

      auto const skip_episode = static_cast<unsigned>(t / motion_length);
      auto const trailing_t = t - skip_episode * motion_length;
      return this->frame_at(trailing_t) + last->second * skip_episode;
    } else {
      throw errors::OutOfFramesError(t);
    }
  } else {
    // Between two frames -> interpolate
    auto const t1 = std::next(l, -1)->first;
    auto const t2 = u->first;
    auto const &f1 = std::next(l, -1)->second;
    auto const &f2 = u->second;
    return interpolate((t - t1) / (t2 - t1), f1, f2);
  }
}

FrameRange Motion::frames(double fps) const { return FrameRange{*this, fps}; }

bool Motion::is_in_range_at(double t) const {
  if (this->impl->loop == LoopType::Wrap) {
    return true;
  } else {
    auto const last_t = std::next(this->impl->raw_frames.end(), -1)->first;
    return t <= last_t;
  }
}

Frame &Motion::get_or_insert_frame(double t) {
  return this->impl->raw_frames[t];
}

LoopType Motion::loop() const { return this->impl->loop; }

void Motion::set_loop(LoopType loop) { this->impl->loop = loop; }

std::string Motion::model_id() const { return this->impl->model_id; }

void Motion::set_model_id(std::string const &model_id) {
  this->impl->model_id = model_id;
}

EffectorType Motion::effector_type(const std::string &name) const {
  return this->impl->effector_types.at(name);
}

void Motion::set_effector_type(const std::string &name, EffectorType type) {
  this->impl->effector_types.at(name) = type;
}

double Motion::length() const {
  return std::next(this->impl->raw_frames.end(), -1)->first;
}

bool Motion::Impl::is_valid() const {
  // for internal use
  //
  // Arbitrary flom::Motion object,
  // which is constructed only using public interface,
  // must not be marked as invalid by this method.
  //
  // TODO: check whether all frames has same names
  return this->raw_frames.size() > 0 && this->raw_frames.begin()->first == 0;
}

KeyRange<std::string> Motion::joint_names() const {
  return this->impl->raw_frames.begin()->second.joint_names();
}

KeyRange<std::string> Motion::effector_names() const {
  return this->impl->raw_frames.begin()->second.effector_names();
}

bool operator==(const Motion &m1, const Motion &m2) {
  return m1.impl->model_id == m2.impl->model_id &&
         m1.impl->loop == m2.impl->loop &&
         m1.impl->raw_frames == m2.impl->raw_frames;
}

} // namespace flom
