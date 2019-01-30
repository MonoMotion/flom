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

option(USE_LIBCXX
  "Use libc++ explicitly"
  OFF)

option(USE_LIBSTDCXX
  "Use libstdc++ explicitly"
  OFF)

function(flom_set_compile_options target)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${target} PUBLIC -pedantic -Wall -Weverything -Wno-float-equal -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-missing-prototypes -Wno-range-loop-analysis)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target} PUBLIC -pedantic -Wall -Wextra -Wno-float-equal -Wno-padded -Wno-unused-parameter -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef)
  endif()

  if(CONFIG STREQUAL "Debug")
    target_compile_options(${target} PUBLIC -O0 -g3)
  elseif(CONFIG STREQUAL "Release")
    target_compile_options(${target} PUBLIC -O3)
  endif()

  if(USE_LIBCXX)
    target_compile_options(${target} PUBLIC -stdlib=libc++)
    target_link_options(${target} PUBLIC -stdlib=libc++)
    target_link_libraries(${target} PUBLIC c++ c++abi)
  elseif(USE_LIBSTDCXX)
    target_compile_options(${target} PUBLIC -stdlib=libstdc++)
    target_link_options(${target} PUBLIC -stdlib=libstdc++)
    target_link_libraries(${target} PUBLIC stdc++)
  endif()

  target_link_libraries(${target} PRIVATE Eigen3::Eigen)
endfunction()
