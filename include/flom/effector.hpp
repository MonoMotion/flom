#ifndef FLOM_EFFECTOR_HPP
#define FLOM_EFFECTOR_HPP

#include <optional>
#include <type_traits>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>
#include <boost/operators.hpp>

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

struct Effector : boost::operators<Effector> {
  std::optional<Location> location;
  std::optional<Rotation> rotation;

  Effector& operator+=(const Effector& x);
  Effector& operator-=(const Effector& x);

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
  Effector& operator*=(T x) {
    if (this->location) {
      this->location->vec *= x;
    }
    if (this->rotation) {
      this->rotation->quat *= x;
    }
    return *this;
  }
};

template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
Effector operator*(const Effector& t1, T t2) { return Effector(t1) *= t2; }

}

#endif
