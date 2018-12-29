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
  class Impl;
  std::unique_ptr<Impl> impl;

  bool is_end = false;

public:
  frame_iterator() noexcept;
  frame_iterator(Motion const &motion, double fps) noexcept;

  frame_iterator(const frame_iterator &);
  frame_iterator(frame_iterator &&);
  frame_iterator &operator=(const frame_iterator &);
  frame_iterator &operator=(frame_iterator &&);

  ~frame_iterator();

  // This is InputIterator because operator* doesn't return reference
  value_type operator*() const;

  frame_iterator &operator++() noexcept;
  frame_iterator operator++(int) noexcept;

  frame_iterator &operator--() noexcept;
  frame_iterator operator--(int) noexcept;

  double current_time() const noexcept;
};

frame_iterator::difference_type operator-(const frame_iterator &,
                                          const frame_iterator &) noexcept;
bool operator==(const frame_iterator &, const frame_iterator &) noexcept;
bool operator!=(const frame_iterator &, const frame_iterator &) noexcept;

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
