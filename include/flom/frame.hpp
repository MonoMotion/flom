#ifndef FLOM_FRAME_HPP
#define FLOM_FRAME_HPP

#include "flom/effector.hpp"

#include <string>
#include <unordered_map>

#include <boost/operators.hpp>
#include <range/v3/view.hpp>

namespace flom {

struct Frame : boost::operators<Frame> {
  std::unordered_map<std::string, double> positions;
  std::unordered_map<std::string, Effector> effectors;

  ranges::any_view<std::string> joint_names() const;
  ranges::any_view<std::string> effector_names() const;

  Frame &operator+=(const Frame &x);
  Frame &operator-=(const Frame &x);

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
  Frame &operator*=(T x) {
    for (auto &&[k, v] : this->positions) {
      v *= x;
    }
    for (auto &&[k, v] : this->effectors) {
      v *= x;
    }
    return *this;
  }
};

template <typename T, std::enable_if_t<std::is_arithmetic_v<T>> * = nullptr>
Frame operator*(const Frame &t1, T t2) {
  return Frame(t1) *= t2;
}

bool operator==(const Frame &, const Frame &);
bool almost_equal(const Frame &, const Frame &);

} // namespace flom

#endif
