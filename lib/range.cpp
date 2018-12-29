#include "flom/range.hpp"
#include "flom/range.impl.hpp"

namespace flom {

frame_iterator::frame_iterator() noexcept : impl(), is_end(true) {}
frame_iterator::frame_iterator(Motion const &motion, double fps) noexcept
    : impl(std::make_unique<Impl>(motion, fps)) {}

frame_iterator::value_type frame_iterator::operator*() const {
  return this->impl->motion->frame_at(this->current_time());
}

frame_iterator &frame_iterator::operator++() noexcept {
  this->impl->t_index++;
  this->is_end = this->impl->check_is_end();
  return *this;
}

frame_iterator frame_iterator::operator++(int) noexcept {
  const auto copy = *this;
  ++(*this);
  return copy;
}

frame_iterator &frame_iterator::operator--() noexcept {
  this->impl->t_index--;
  this->is_end = this->impl->check_is_end();
  return *this;
}

frame_iterator frame_iterator::operator--(int) noexcept {
  const auto copy = *this;
  --(*this);
  return copy;
}

double frame_iterator::Impl::current_time() const noexcept {
  return this->fps * this->t_index;
}

double frame_iterator::current_time() const noexcept {
  return this->impl->current_time();
}

bool frame_iterator::Impl::check_is_end() const noexcept {
  return !this->motion->is_in_range_at(this->current_time());
}

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

} // namespace flom
