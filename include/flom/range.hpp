#ifndef FLOM_RANGE_HPP
#define FLOM_RANGE_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include <iterator>
#include <memory>

namespace flom {

// using snake_case, following customs of iterator naming
class frame_iterator {
public:
  using iterator_category = std::forward_iterator_tag;
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

  value_type operator*() {
    return this->motion->frame_at(this->fps * this->t_index);
  }

  frame_iterator &operator++() noexcept {
    this->t_index++;
    this->is_end = !this->motion->is_in_range_at(this->fps * this->t_index);
    return *this;
  }

  constexpr bool operator==(const frame_iterator &r) const noexcept {
    return this->is_end == r.is_end;
  }
  constexpr bool operator!=(const frame_iterator &r) const noexcept {
    return !(*this == r);
  }
};

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
