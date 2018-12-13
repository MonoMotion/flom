#ifndef FLOM_RANGE_HPP
#define FLOM_RANGE_HPP

#include "flom/frame.hpp"

#include <iterator>

namespace flom {

// frame_iterator is based on random_generator_iterator.hpp
// in https://github.com/yumetodo/random_generator_iterator,
// which is distributed under the Boost Software License, Version 1.0.
//
// Copyright (C) 2016 yumetodo <yume-wikijp@live.jp>
// (See http://www.boost.org/LICENSE_1_0.txt)
//
// using snake_case, following customs of iterator naming
class frame_iterafor
{
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Frame;
  using difference_type = double;
  using pointer = Frame*;
  using reference = Frame&;

private:
  struct Impl {
    std::reference_wrapper<Frame> frame;
    double fps;
    long t_index;
    bool next_is_end;
    Impl() = default;
    Impl(const Impl&) = delete;
    Impl(Impl&&) = default;
    Impl& operator=(const Impl&) = delete;
    Impl& operator=(Impl&&) = default;
    Impl(Frame& frame, double fps) : fps(fps), frame(frame), next_is_end(false) {}
    value_type get() {
      return this->frame.frame_at(this->fps * this->t_index);
    }
  };

  std::unique_ptr<Impl> pimpl;
  bool is_end_iterator;

public:
  constexpr frame_iterator() noexcept : pimpl(), is_end_iterator(true) {};
  frame_iterator(const frame_iterator&) = delete;
  frame_iterator(frame_iterator&&) = default;
  frame_iterator& operator=(const frame_iterator&) = delete;
  frame_iterator& operator=(frame_iterator&&) = default;

  frame_iterator(Frame& frame, double fps)
    : pimpl(std::make_unique<Impl>(frame, fps)), is_end_iterator(false)
  {}

  void stop() noexcept { this->pimpl->next_is_end = true; }

  value_type operator*() { return this->pimpl->get(); }

  frame_iterator& operator++() noexcept
  {
    if (this->pimpl->next_is_end) this->is_end_iterator = true;
    this->pimpl->t_index++;
    return *this;
  }
  frame_iterator operator++(int) noexcept
  {
    const auto re = *this;
    if (this->pimpl->next_is_end) this->is_end_iterator = true;
    this->pimpl->t_index++;
    return re;
  }

  constexpr bool operator==(const frame_iterator& r) const noexcept { return this->is_end_iterator == r.is_end_iterator; }
  constexpr bool operator!=(const frame_iterator& r) const noexcept { return !(*this == r); }
};

class FrameRange {
public:
  using value_type = Frame;
  using iterator = frame_iterator;

private:
  Frame& frame;
  double fps;

public:
  FrameRange() = delete;
  FrameRange(Frame& f, double fps) : frame(f), fps(fps) {}
  FrameRange(const FrameRange&) = default;
  FrameRange(FrameRange&&) = default;
  FrameRange& operator=(const FrameRange&) = default;
  FrameRange& operator=(FrameRange&&) = default;

  iterator begin() noexcept { return { this->f, this->fps }; }
  iterator end() noexcept { return {}; }
};

}

#endif
