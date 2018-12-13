#ifndef FLOM_FRAME_HPP
#define FLOM_FRAME_HPP

#include <optional>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>

namespace flom {

struct Effect {
  std::optional<Translation> translation;
  std::optional<Rotation> rotation;
};

Effect interpolate(double t, Effect const& a, Effect const& b);

using qvm = boost::qvm;

struct Translation {
  float weight;
  qvm::vec<double, 3> vec;
};

struct Rotation {
  float weight;
  qvm::quat<double> quat;
};

}

#endif

