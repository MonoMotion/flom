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

add_library(flom_lib SHARED motion.cpp motion_io.cpp frame.cpp effector.cpp proto_util.cpp errors.cpp)
add_dependencies(flom_lib flom_headers)
flom_set_compile_options(flom_lib)
set_target_properties(flom_lib PROPERTIES OUTPUT_NAME "flom")
set_target_properties(flom_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(flom_lib PRIVATE flom_proto nlohmann_json::nlohmann_json ${PROTOBUF_LIBRARIES})
enable_clang_format(flom_lib)
enable_clang_tidy(flom_lib)
