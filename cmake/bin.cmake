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

cmake_minimum_required(VERSION 3.0.2)

function(flom_add_bin target)
  add_executable(${target}_bin ${target}.cpp)
  set_target_properties(${target}_bin PROPERTIES OUTPUT_NAME ${target})
  flom_set_compile_options(${target}_bin)
  # TODO: Don't link protobuf libraries here!
  target_link_libraries(${target}_bin PRIVATE ${PROTOBUF_LIBRARIES} flom_lib)
  enable_clang_format(${target}_bin)
  enable_clang_tidy(${target}_bin)
endfunction()
