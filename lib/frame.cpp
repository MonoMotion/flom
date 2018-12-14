#include "flom/frame.hpp"
#include "flom/interpolation.hpp"

namespace flom {

Frame interpolate(double t, Frame const &a, Frame const &b) {
  Frame f;
  for (auto const &[k, v1] : a.positions) {
    auto const v2 = b.positions.at(k);
    f.positions[k] = lerp(t, v1, v2);
  }
  for (auto const &[k, v1] : a.effectors) {
    auto const v2 = b.effectors.at(k);
    f.effectors[k] = interpolate(t, v1, v2);
  }
  return f;
}

Frame &Frame::operator+=(const Frame &x) {
  for (auto &&[k, v1] : this->positions) {
    auto const &v2 = x.positions.at(k);
    v1 += v2;
  }
  for (auto &&[k, v1] : this->effectors) {
    auto const &v2 = x.effectors.at(k);
    v1 += v2;
  }
  return *this;
}

Frame &Frame::operator-=(const Frame &x) {
  for (auto &&[k, v1] : this->positions) {
    auto const &v2 = x.positions.at(k);
    v1 -= v2;
  }
  for (auto &&[k, v1] : this->effectors) {
    auto const &v2 = x.effectors.at(k);
    v1 -= v2;
  }
  return *this;
}

} // namespace flom
