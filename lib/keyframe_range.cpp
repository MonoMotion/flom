#include "flom/range.hpp"
#include "flom/range.impl.hpp"

namespace flom {

bool operator==(const keyframe_iterator &l,
                const keyframe_iterator &r) noexcept {
  return l.it == r.it;
}

bool operator!=(const keyframe_iterator &l,
                const keyframe_iterator &r) noexcept {
  return !(l == r);
}

} // namespace flom
