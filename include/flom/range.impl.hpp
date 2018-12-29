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

#ifndef FLOM_RANGE_IMPL_HPP
#define FLOM_RANGE_IMPL_HPP

namespace flom {

class frame_iterator::Impl {
public:
  const Motion *motion;
  double fps;
  long t_index = 0;

  Impl(const Motion &motion_, double fps_) : motion(&motion_), fps(fps_) {}

  double current_time() const noexcept;
  bool check_is_end() const noexcept;
};

} // namespace flom

#endif
