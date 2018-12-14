#ifndef FLOM_TEST_PRINTERS_HPP
#define FLOM_TEST_PRINTERS_HPP

#include <boost/qvm/vec.hpp>
#include <boost/qvm/quat.hpp>
#include <boost/qvm/quat_access.hpp>

namespace boost::qvm {

template<class T>
std::ostream &operator<<(std::ostream &os, const quat<T> &q) {
  os << "quaternion(" << S(q) << "," << X(q) << "," << Y(q) << "," << Z(q)
     << ")";
  return os;
}

template<class T, int Dim>
std::ostream &operator<<(std::ostream &os, const vec<T, Dim> &v) {
  os << "vec(";
  for(T e : v.a) {
    os << e << ",";
  }
  os << ")";
  return os;
}

} // namespace boost::qvm

#endif
