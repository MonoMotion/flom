#include "flom/frame.hpp"
#include "flom/interpolation.hpp"

namespace flom {

Frame interpolate(double t, Frame const& a, Frame const& b) {
  Frame f;
  for (auto const& [k, v1] : a.positions) {
    auto const v2 = b.positions.at(k);
    f.positions[k] = lerp(t, v1, v2);
  }
  for (auto const& [k, v1] : a.effectors) {
    auto const v2 = b.effectors.at(k);
    f.effectors[k] = interpolate(t, v1, v2);
  }
  return std::move(f);
}

}
