#ifndef FLOM_RANGE_HPP
#define FLOM_RANGE_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include <iterator>
#include <memory>

namespace flom {

// frame_iterator is based on random_generator_iterator.hpp
// in https://github.com/yumetodo/random_generator_iterator,
// which is distributed under the Boost Software License, Version 1.0.
//
// Copyright (C) 2016 yumetodo <yume-wikijp@live.jp>
// (See http://www.boost.org/LICENSE_1_0.txt)
//
// using snake_case, following customs of iterator naming
class frame_iterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Frame;
  using difference_type = double;
  using pointer = Frame *;
  using reference = Frame &;

private:
  struct Impl {
    std::reference_wrapper<const Motion> motion;
    double fps;
    long t_index;
    bool next_is_end;
    Impl() = default;
    Impl(const Impl &) = delete;
    Impl(Impl &&) = default;
    Impl &operator=(const Impl &) = delete;
    Impl &operator=(Impl &&) = default;
    Impl(Motion const &motion_, double fps_)
        : motion(motion_), fps(fps_), t_index(0), next_is_end(false) {}
    value_type get() {
      return this->motion.get().frame_at(this->fps * this->t_index);
    }
    bool check_bound() const {
      return this->motion.get().is_in_range_at(this->fps * this->t_index);
    }
  };

  std::unique_ptr<Impl> pimpl;
  bool is_end_iterator;

public:
  constexpr frame_iterator() noexcept : pimpl(), is_end_iterator(true) {}
  frame_iterator(const frame_iterator &) = delete;
  frame_iterator(frame_iterator &&) = default;
  frame_iterator &operator=(const frame_iterator &) = delete;
  frame_iterator &operator=(frame_iterator &&) = default;

  frame_iterator(Motion const &motion, double fps)
      : pimpl(std::make_unique<Impl>(motion, fps)), is_end_iterator(false) {}

  void stop() noexcept { this->pimpl->next_is_end = true; }

  value_type operator*() { return this->pimpl->get(); }

  frame_iterator &operator++() noexcept {
    if (this->pimpl->next_is_end)
      this->is_end_iterator = true;
    this->pimpl->t_index++;
    this->pimpl->next_is_end = !this->pimpl->check_bound();
    return *this;
  }

  constexpr bool operator==(const frame_iterator &r) const noexcept {
    return this->is_end_iterator == r.is_end_iterator;
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
