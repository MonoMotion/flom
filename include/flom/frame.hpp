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
  friend struct Frame;
  friend bool operator==(const FrameDifference &, const FrameDifference &);

private:
  std::unordered_map<std::string, double> positions;
  std::unordered_map<std::string, EffectorDifference> effectors;

public:
  FrameDifference(const Frame &, const Frame &);
  FrameDifference(const std::unordered_map<std::string, EffectorType> &,
                  const Frame &, const Frame &);

  FrameDifference() = delete;

  FrameDifference(const FrameDifference &) = default;
  FrameDifference(FrameDifference &&) = default;

  FrameDifference &operator=(const FrameDifference &) = default;
  FrameDifference &operator=(FrameDifference &&) = default;

  FrameDifference &operator*=(std::size_t);
  FrameDifference &operator+=(const FrameDifference &);
};

bool operator==(const FrameDifference &, const FrameDifference &);

struct Frame : boost::addable<Frame, FrameDifference> {
  std::unordered_map<std::string, double> positions;
  std::unordered_map<std::string, Effector> effectors;

  KeyRange<std::string> joint_names() const;
  KeyRange<std::string> effector_names() const;

  // TODO: Add is_compatible and test for this using is_compatible
  Frame new_compatible_frame() const;

  Frame &operator+=(const FrameDifference &);
};

FrameDifference operator-(const Frame &, const Frame &);
bool operator==(const Frame &, const Frame &);
bool operator!=(const Frame &, const Frame &);
bool almost_equal(const Frame &, const Frame &);

} // namespace flom

#endif
