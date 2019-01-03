//
// Copyright 2018 coord.e
//
// This file is part of Flom.
//
// Flom is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Flom is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Flom.  If not, see <http://www.gnu.org/licenses/>.
//

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

struct Location {
  using value_type = qvm::vec<double, 3>;

  double weight;
  value_type vec;

  Location() : weight(0) {}
};

bool operator==(const Location &, const Location &);
bool operator!=(const Location &, const Location &);
bool almost_equal(const Location &, const Location &);

struct Rotation {
  using value_type = qvm::quat<double>;

  double weight;
  value_type quat;

  Rotation() : weight(0) {}
};

bool operator==(const Rotation &, const Rotation &);
bool operator!=(const Rotation &, const Rotation &);
bool almost_equal(const Rotation &, const Rotation &);

struct Effector;

class EffectorDifference : boost::operators<EffectorDifference> {
  friend struct Effector;
  friend EffectorDifference operator-(const Effector &, const Effector&);

private:
  std::optional<Location::value_type> location;
  std::optional<Rotation::value_type> rotation;

  EffectorDifference(const Effector &, const Effector &);

public:
  EffectorDifference() = delete;

  EffectorDifference(const EffectorDifference &) = default;
  EffectorDifference(EffectorDifference &&) = default;

  EffectorDifference &operator=(const EffectorDifference &) = default;
  EffectorDifference &operator=(EffectorDifference &&) = default;

  EffectorDifference &operator*=(std::size_t);
  EffectorDifference &operator+=(const EffectorDifference &);
};

struct Effector : boost::operators<Effector> {
  std::optional<Location> location;
  std::optional<Rotation> rotation;

  Effector &operator+=(const Effector &);
  Effector &operator+=(const EffectorDifference &);
};

bool operator==(const Effector &, const Effector &);
bool operator!=(const Effector &, const Effector &);
bool almost_equal(const Effector &, const Effector &);
EffectorDifference operator-(const Effector &, const Effector &);

bool almost_equal(double, double);

} // namespace flom

#endif
