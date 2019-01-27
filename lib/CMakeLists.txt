#
# Copyright 2018 coord.e
#
# This file is part of Flom.
#
# Flom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Flom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Flom.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.7)

option(BUILD_SHARED_LIB "Build a shared library" ON)
option(BUILD_STATIC_LIB "Build a static library" ON)

set(flom_lib_files motion.cpp motion_io.cpp frame.cpp effector.cpp proto_util.cpp errors.cpp frame_range.cpp keyframe_range.cpp effector_type.cpp effector_weight.cpp loose_compare.cpp)

if(BUILD_SHARED_LIB)
  add_library(flom_lib SHARED ${flom_lib_files})
  add_dependencies(flom_lib flom_headers)
  flom_add_lib(flom_lib flom)
  install(TARGETS flom_lib LIBRARY DESTINATION lib)
endif()

if(BUILD_STATIC_LIB)
  add_library(flom_lib_static STATIC ${flom_lib_files})
  add_dependencies(flom_lib_static flom_headers)
  flom_add_lib(flom_lib_static flom)
  install(TARGETS flom_lib_static ARCHIVE DESTINATION lib)
endif()

enable_clang_format(flom_lib)
enable_clang_tidy(flom_lib)
