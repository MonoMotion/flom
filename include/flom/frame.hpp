#ifndef FLOM_FRAME_HPP
#define FLOM_FRAME_HPP

#include "flom/effect.hpp"

namespace flom {

struct Frame {
  std::unordered_map<std::string, double> changes;
  std::unordered_map<std::string, Effect> effects;
};

Frame interpolate(double, Frame, Frame);

}

#endif
