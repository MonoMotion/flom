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

#ifndef FLOM_FRAME_HPP
#define FLOM_FRAME_HPP

#include "flom/effector.hpp"

#include <string>
#include <unordered_map>

#include <boost/operators.hpp>
#include <boost/range/any_range.hpp>

namespace flom {

// TODO: Hide Boost.Range
template <typename K>
using KeyRange =
    boost::any_range<K, boost::forward_traversal_tag,
                     std::add_lvalue_reference_t<K>, std::ptrdiff_t>;

struct Frame;

class FrameDifference
    : boost::addable<FrameDifference,
                     boost::equality_comparable<
                         FrameDifference,
                         boost::multipliable<FrameDifference, std::size_t>>> {

private:
  std::unordered_map<std::string, double> positions_;
  std::unordered_map<std::string, EffectorDifference> effectors_;

public:
  FrameDifference(const Frame &, const Frame &);

  FrameDifference() = delete;

  FrameDifference(const FrameDifference &) = default;
  FrameDifference(FrameDifference &&) = default;

  FrameDifference &operator=(const FrameDifference &) = default;
  FrameDifference &operator=(FrameDifference &&) = default;

  const std::unordered_map<std::string, double> &positions() const &;
  std::unordered_map<std::string, double> positions() &&;

  const std::unordered_map<std::string, EffectorDifference> &
  effectors() const &;
  std::unordered_map<std::string, EffectorDifference> effectors() &&;

  FrameDifference &operator*=(std::size_t);
  FrameDifference &operator+=(const FrameDifference &);

  bool is_compatible(const FrameDifference &) const;
};

bool operator==(const FrameDifference &, const FrameDifference &);
bool almost_equal(const FrameDifference &, const FrameDifference &);

struct Frame : boost::addable<Frame, FrameDifference> {
private:
  using PositionsMap = std::unordered_map<std::string, double>;
  using EffectorsMap = std::unordered_map<std::string, Effector>;

  PositionsMap positions_;
  EffectorsMap effectors_;

public:
  Frame();
  Frame(const PositionsMap &, const EffectorsMap &);

  const PositionsMap &positions() const &;
  PositionsMap positions() &&;

  void set_positions(const PositionsMap &);
  void set_position(const std::string &, double);

  const EffectorsMap &effectors() const &;
  EffectorsMap effectors() &&;

  void set_effectors(const EffectorsMap &);
  void set_effector(const std::string &, const Effector &);

  KeyRange<std::string> joint_names() const;
  KeyRange<std::string> effector_names() const;

  Frame new_compatible_frame() const;
  bool is_compatible(const Frame &) const;
  bool is_compatible(const FrameDifference &) const;

  Frame &operator+=(const FrameDifference &);
};

FrameDifference operator-(const Frame &, const Frame &);
bool operator==(const Frame &, const Frame &);
bool operator!=(const Frame &, const Frame &);
bool almost_equal(const Frame &, const Frame &);

} // namespace flom

#endif
