#ifndef FLOM_RANGE_HPP
#define FLOM_RANGE_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include <iterator>
#include <type_traits>
#include <utility>
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
    this->pimpl->t_index++;
    this->pimpl->next_is_end = !this->pimpl->check_bound();
    if (this->pimpl->next_is_end)
      this->is_end_iterator = true;
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


template<int Idx, typename Base>
class tuple_iterator {
public:
  using tuple_type = typename std::iterator_traits<Base>::value_type;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename std::tuple_element<Idx, tuple_type>::type;
  using difference_type = typename std::iterator_traits<Base>::difference_type;
  using pointer = typename std::add_pointer_t<value_type>;
  using reference = typename std::add_lvalue_reference_t<value_type>;

  tuple_iterator(Base it_) : it(it_) {}

  value_type operator*() const { return std::get<Idx>(*this->it); }
  tuple_iterator& operator++() noexcept {
    this->it ++;
    return *this;
  }

  tuple_iterator& operator--() noexcept {
    this->it --;
    return *this;
  }

  pointer operator->() const noexcept {
    return &std::get<Idx>(*this->it);
  }

  constexpr bool operator==(const tuple_iterator& t) const noexcept {
    return this->it == t.it;
  }

  constexpr bool operator!=(const tuple_iterator& t) const noexcept {
    return !(*this == t);
  }
private:
  Base it;
};

template<int Idx, typename T>
class TupleRange {
public:
  using value_type = typename std::tuple_element<Idx, typename T::value_type>::type;
  using iterator = tuple_iterator<Idx, typename T::iterator>;
  using const_iterator = tuple_iterator<Idx, typename T::const_iterator>;

private:
  T& v;

public:
  TupleRange() = delete;
  TupleRange(T& v_) : v(v_) {}
  TupleRange(const TupleRange &) = default;
  TupleRange(TupleRange &&) = default;
  TupleRange &operator=(const TupleRange &) = default;
  TupleRange &operator=(TupleRange &&) = default;

  iterator begin() noexcept { return {std::begin(this->v)}; }
  iterator end() noexcept { return {std::end(this->v)}; }

  const_iterator cbegin() const noexcept { return {std::cbegin(this->v)}; }
  const_iterator cend() const noexcept { return {std::cend(this->v)}; }
};

// TODO: Delete V
// TODO: Support map / unordered_map
template<typename K, typename V>
using KeyRange = TupleRange<0, std::unordered_map<K, V>>;

} // namespace flom

#endif
