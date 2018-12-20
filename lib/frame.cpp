#include "flom/frame.hpp"
#include "flom/interpolation.hpp"

#include <boost/range/adaptors.hpp>

#include <algorithm>

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

bool operator==(const Frame &f1, const Frame &f2) {
  return f1.positions == f2.positions && f1.effectors == f2.effectors;
}

bool almost_equal(const Frame &f1, const Frame &f2) {
  auto p = std::all_of(std::cbegin(f1.positions), std::cend(f1.positions),
                       [&f2](auto const &pair) {
                         auto const &[joint, pos1] = pair;
                         auto const pos2 = f2.positions.at(joint);
                         return almost_equal(pos1, pos2);
                       });
  auto e = std::all_of(std::cbegin(f1.effectors), std::cend(f1.effectors),
                       [&f2](auto const &pair) {
                         auto const &[link, e1] = pair;
                         auto const e2 = f2.effectors.at(link);
                         return almost_equal(e1, e2);
                       });
  return p && e;
}

KeyRange<std::string> Frame::joint_names() const {
  return this->positions | boost::adaptors::map_keys;
}

KeyRange<std::string> Frame::effector_names() const {
  return this->effectors | boost::adaptors::map_keys;
}

} // namespace flom
