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

file(GLOB test_lib_files *.cpp)
add_library(flom_test_lib SHARED ${test_lib_files})
add_dependencies(flom_test_lib flom_test_headers)
flom_set_compile_options(flom_test_lib)
  set_target_properties(flom_test_lib PROPERTIES OUTPUT_NAME "flom_test")
target_link_libraries(flom_test_lib PRIVATE flom_lib)
