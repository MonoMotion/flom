#include "flom/range.hpp"

namespace flom {

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
