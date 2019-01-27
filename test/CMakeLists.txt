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

include_directories(include)
add_subdirectory(include)

add_subdirectory(lib)

function(flom_add_test target)
  target_link_libraries(${target} PRIVATE flom_lib flom_test_lib rapidcheck)
  add_dependencies(${target} flom_test_headers)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${target} PUBLIC -Wno-global-constructors -Wno-disabled-macro-expansion)
  endif()
  add_test(NAME ${target} COMMAND ${target})
  enable_clang_format(${target})
  enable_clang_tidy(${target})
endfunction()

add_subdirectory(bin)
