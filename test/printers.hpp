#ifndef FLOM_TEST_PRINTERS_HPP
#define FLOM_TEST_PRINTERS_HPP

#include <flom/flom.hpp>

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

namespace flom {

std::ostream &operator<<(std::ostream &os, const Rotation &v) {
  os << "rotation(" << v.weight << ", " << v.quat << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Location &v) {
  os << "location(" << v.weight << ", " << v.vec << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Effector &v) {
  os << "effector(";
  if(v.location) {
    os << *v.location << ",";
  }
  if(v.rotation) {
    os << *v.rotation << ",";
  }
  os << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Frame &v) {
  os << "frame(\n";
  os << "positions(";
  for(auto const& [j, p] : v.positions) {
   os << j << ": " << p << ", ";
  }
  os << ")\neffectors(";
  for(auto const& [l, e] : v.effectors) {
   os << l << ": " << e << ", ";
  }
  os << "))";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Motion &v) {
  auto s = v.dump_json_string();
  os << s;
  return os;
}

}

#endif
