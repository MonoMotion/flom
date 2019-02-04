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

#include "flom/optional.hpp"
#include <type_traits>

#include <boost/operators.hpp>

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace flom {

class Location
    : boost::addable<
          Location,
          boost::subtractable<
              Location,
              boost::equality_comparable<
                  Location, boost::multipliable<Location, std::size_t>>>> {
public:
  using value_type = Eigen::Matrix<double, 3, 1>;

private:
  value_type vector_;

public:
  Location();
  Location(double x, double y, double z);

  explicit Location(const value_type &);

  const value_type &vector() const;
  void set_vector(const value_type &);

  double x() const;
  double y() const;
  double z() const;

  std::tuple<double, double, double> xyz() const;

  void set_x(double);
  void set_y(double);
  void set_z(double);

  void set_xyz(double, double, double);

  Location &operator+=(const Location &);
  Location &operator-=(const Location &);
  Location &operator*=(std::size_t);
};

bool operator==(const Location &, const Location &);

struct Rotation
    : boost::addable<
          Rotation,
          boost::subtractable<
              Rotation,
              boost::equality_comparable<
                  Rotation, boost::multipliable<Rotation, std::size_t>>>> {
public:
  using value_type = Eigen::Quaternion<double>;

private:
  value_type quat_;

public:
  Rotation();
  Rotation(double w, double x, double y, double z);

  explicit Rotation(const value_type &);

  const value_type &quaternion() const;
  void set_quaternion(const value_type &);

  double w() const;
  double x() const;
  double y() const;
  double z() const;

  std::tuple<double, double, double, double> wxyz() const;

  void set_wxyz(double, double, double, double);

  Rotation &operator+=(const Rotation &);
  Rotation &operator-=(const Rotation &);
  Rotation &operator*=(std::size_t);
};

bool operator==(const Rotation &, const Rotation &);

struct Effector;

class EffectorDifference
    : boost::addable<
          EffectorDifference,
          boost::equality_comparable<
              EffectorDifference,
              boost::multipliable<EffectorDifference, std::size_t>>> {
private:
  optional::optional<Location> location_;
  optional::optional<Rotation> rotation_;

public:
  EffectorDifference(const Effector &, const Effector &);

  EffectorDifference() = delete;

  EffectorDifference(const EffectorDifference &) = default;
  EffectorDifference(EffectorDifference &&) = default;

  EffectorDifference &operator=(const EffectorDifference &) = default;
  EffectorDifference &operator=(EffectorDifference &&) = default;

  const optional::optional<Location> &location() const &;
  optional::optional<Location> location() &&;

  const optional::optional<Rotation> &rotation() const &;
  optional::optional<Rotation> rotation() &&;

  EffectorDifference &operator*=(std::size_t);
  EffectorDifference &operator+=(const EffectorDifference &);

  bool is_compatible(const EffectorDifference &) const;
};

bool operator==(const EffectorDifference &, const EffectorDifference &);

struct Effector : boost::addable<Effector, EffectorDifference> {
private:
  optional::optional<Location> location_;
  optional::optional<Rotation> rotation_;

public:
  Effector();
  Effector(const optional::optional<Location> &,
           const optional::optional<Rotation> &);

  const optional::optional<Location> &location() const &;
  optional::optional<Location> location() &&;

  void set_location(const optional::optional<Location> &);
  void clear_location();

  const optional::optional<Rotation> &rotation() const &;
  optional::optional<Rotation> rotation() &&;

  void set_rotation(const optional::optional<Rotation> &);
  void clear_rotation();

  Effector new_compatible_effector() const;
  bool is_compatible(const Effector &) const;
  bool is_compatible(const EffectorDifference &) const;

  Effector &operator+=(const EffectorDifference &);
};

bool operator==(const Effector &, const Effector &);
bool operator!=(const Effector &, const Effector &);
EffectorDifference operator-(const Effector &, const Effector &);

} // namespace flom

#endif
