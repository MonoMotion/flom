#include "flom/motion.hpp"

namespace flom {

Frame const& frame_at(double t) const {
  auto const [l, u] = this->frames.equal_range(t);
  if (l->first == t) {
    return l->second;
  } else if (u == m.end()) {
    // TBD
  } else {
    auto const t1 = std::next(l, -1)->first;
    auto const t2 = u->first;
    auto const& f1 = std::next(l, -1)->second;
    auto const& f2 = u->second;
    return interpolate((t - t1) / (t2 - t1), f1, f2);
  }
}

Motion Motion::load(std::ifstream& f) {
  throw std::runtime_error("Not implemented");
}

void dump(std::ofstream&) {
  throw std::runtime_error("Not implemented");
}

}
