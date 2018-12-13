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

struct Translation {
  float weight;
  CoordinateSystem coord_system;
  qvm::vec<double, 3> vec;

  Translation() : weight(0) {}
};

Translation interpolate(double t, Translation const& a, Translation const& b);

struct Rotation {
  float weight;
  CoordinateSystem coord_system;
  qvm::quat<double> quat;

  Rotation() : weight(0) {}
};

Rotation interpolate(double t, Rotation const& a, Rotation const& b);

template<typename T>
T lerp(double t, T a, T b) {
  return a + t * (b - a);
}

struct Effect {
  std::optional<Translation> translation;
  std::optional<Rotation> rotation;
};

Effect interpolate(double t, Effect const& a, Effect const& b);

}

#endif

