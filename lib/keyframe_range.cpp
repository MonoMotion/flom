#include "flom/range.hpp"
#include "flom/range.impl.hpp"

namespace flom {

const keyframe_iterator::value_type &keyframe_iterator::operator*() const {
  return *this->it;
}
keyframe_iterator::checked_value_type keyframe_iterator::operator*() {
  return std::make_pair(this->it->first,
                        CheckedFrameRef{this->it->second, this->motion});
}

const keyframe_iterator::value_type &keyframe_iterator::operator->() const {
  return *this->it;
}
keyframe_iterator::checked_value_type keyframe_iterator::operator->() {
  return std::make_pair(this->it->first,
                        CheckedFrameRef{this->it->second, this->motion});
}

keyframe_iterator &keyframe_iterator::operator++() noexcept {
  this->it++;
  return *this;
}
keyframe_iterator keyframe_iterator::operator++(int) noexcept {
  auto const copy = *this;
  ++(*this);
  return copy;
}

keyframe_iterator &keyframe_iterator::operator--() noexcept {
  this->it--;
  return *this;
}
keyframe_iterator keyframe_iterator::operator--(int) noexcept {
  auto const copy = *this;
  --(*this);
  return copy;
}

bool operator==(const keyframe_iterator &l,
                const keyframe_iterator &r) noexcept {
  return l.it == r.it;
}

bool operator!=(const keyframe_iterator &l,
                const keyframe_iterator &r) noexcept {
  return !(l == r);
}

} // namespace flom
