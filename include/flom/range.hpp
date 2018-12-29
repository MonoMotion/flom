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

  difference_type operator-(const frame_iterator &r) const noexcept {
    return this->current_time() - r.current_time();
  }

  constexpr bool operator==(const frame_iterator &r) const noexcept {
    return this->is_end == r.is_end;
  }
  constexpr bool operator!=(const frame_iterator &r) const noexcept {
    return !(*this == r);
  }

private:
  bool check_is_end() const noexcept {
    return !this->motion->is_in_range_at(this->current_time());
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
