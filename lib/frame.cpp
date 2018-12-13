#include "flom/frame.hpp"

namespace flom {

Frame interpolate(double t, Frame const& a, Frame const& b) {
  Frame f;
  for (auto const& [k, v1] : a.changes) {
    auto const v2 = b.changes.at(k);
    f.changes[k] = lerp(t, v1, v2);
  }
  for (auto const& [k, v1] : a.effects) {
    auto const v2 = b.effects.at(k);
    f.effects[k] = interpolate(t, v1, v2);
  }
  return std::move(f);
}

}
