#ifndef FLOM_EFFECT_HPP
#define FLOM_EFFECT_HPP

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

Translation interpolate(double t, Translation const& a, Translation const& b);

struct Rotation {
  float weight;
  qvm::quat<double> quat;
};

Rotation interpolate(double t, Rotation const& a, Rotation const& b);

template<typename T>
T lerp(double t, T a, T b) {
  return a + t * (b - a);
}

}

#endif

