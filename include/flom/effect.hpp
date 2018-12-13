#ifndef FLOM_EFFECT_HPP
#define FLOM_EFFECT_HPP

#include <optional>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>

namespace flom {

namespace qvm = boost::qvm;

enum class CoordinateSystem {
  World,
  Local,
};

struct Location {
  float weight;
  CoordinateSystem coord_system;
  qvm::vec<double, 3> vec;

  Location() : weight(0) {}
};

struct Rotation {
  float weight;
  CoordinateSystem coord_system;
  qvm::quat<double> quat;

  Rotation() : weight(0) {}
};

struct Effector {
  std::optional<Location> location;
  std::optional<Rotation> rotation;
};

}

#endif

