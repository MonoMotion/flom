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

#ifndef FLOM_RANGE_HPP
#define FLOM_RANGE_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include <iterator>
#include <memory>

namespace flom {

// using snake_case, following customs of iterator naming
class frame_iterator {
  friend bool operator==(const frame_iterator &,
                         const frame_iterator &) noexcept;

public:
  using iterator_category = std::input_iterator_tag;
  using value_type = Frame;
  using difference_type = double;
  using pointer = Frame *;
  using reference = Frame &;

private:
  // DefaultConstructible is required
  const Motion *motion;
  bool is_end = false;
  double fps = 1;
  long t_index = 0;

public:
  constexpr frame_iterator() noexcept : motion(), is_end(true) {}
  frame_iterator(Motion const &motion_, double fps_) noexcept
      : motion(&motion_), fps(fps_) {}

  frame_iterator(const frame_iterator &) = default;
  frame_iterator(frame_iterator &&) = default;
  frame_iterator &operator=(const frame_iterator &) = default;
  frame_iterator &operator=(frame_iterator &&) = default;

  // This is InputIterator because operator* doesn't return reference
  value_type operator*() const {
    return this->motion->frame_at(this->current_time());
  }

  frame_iterator &operator++() noexcept {
    this->t_index++;
    this->is_end = this->check_is_end();
    return *this;
  }

  frame_iterator operator++(int) noexcept {
    const auto copy = *this;
    ++(*this);
    return copy;
  }

  frame_iterator &operator--() noexcept {
    this->t_index--;
    this->is_end = this->check_is_end();
    return *this;
  }

  frame_iterator operator--(int) noexcept {
    const auto copy = *this;
    --(*this);
    return copy;
  }

  double current_time() const noexcept { return this->fps * this->t_index; }

private:
  bool check_is_end() const noexcept {
    return !this->motion->is_in_range_at(this->current_time());
  }
};

frame_iterator::difference_type operator-(const frame_iterator &l,
                                          const frame_iterator &r) noexcept {
  return l.current_time() - r.current_time();
}

bool operator==(const frame_iterator &l, const frame_iterator &r) noexcept {
  return l.is_end == r.is_end;
}

bool operator!=(const frame_iterator &l, const frame_iterator &r) noexcept {
  return !(l == r);
}

class FrameRange {
public:
  using value_type = Frame;
  using iterator = frame_iterator;

private:
  Motion const &motion;
  double fps;

public:
  FrameRange() = delete;
  FrameRange(Motion const &motion_, double fps_) : motion(motion_), fps(fps_) {}
  FrameRange(const FrameRange &) = default;
  FrameRange(FrameRange &&) = default;
  FrameRange &operator=(const FrameRange &) = default;
  FrameRange &operator=(FrameRange &&) = default;

  iterator begin() noexcept { return {this->motion, this->fps}; }
  iterator end() noexcept { return {}; }
};

} // namespace flom

#endif
