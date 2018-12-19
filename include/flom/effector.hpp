#ifndef FLOM_EFFECTOR_HPP
#define FLOM_EFFECTOR_HPP

#include <optional>
#include <type_traits>

#include <boost/operators.hpp>
#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_operations.hpp>
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace flom {

namespace qvm = boost::qvm;

struct Location : boost::operators<Location> {
  double weight;
  qvm::vec<double, 3> vec;

  Location() : weight(0) {}
};

bool operator==(const Location &, const Location &);
bool almost_equal(const Location &, const Location &);

struct Rotation : boost::operators<Rotation> {
  double weight;
  qvm::quat<double> quat;

  Rotation() : weight(0) {}
};

bool operator==(const Rotation &, const Rotation &);
bool almost_equal(const Rotation &, const Rotation &);

struct Effector : boost::operators<Effector> {
  std::optional<Location> location;
  std::optional<Rotation> rotation;

  Effector &operator+=(const Effector &x);
  Effector &operator-=(const Effector &x);

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
  Effector &operator*=(T x) {
    if (this->location) {
      this->location->vec *= x;
    }
    if (this->rotation) {
      this->rotation->quat *= x;
    }
    return *this;
  }
};

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
Effector operator*(const Effector &t1, T t2) {
  return Effector(t1) *= t2;
}

bool operator==(const Effector &, const Effector &);
bool almost_equal(const Effector &, const Effector &);

bool almost_equal(double, double);

} // namespace flom

#endif
