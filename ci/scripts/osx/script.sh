#!/bin/bash
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

mkdir build && cd $_
cmake .. -DCMAKE_CXX_COMPILER=${COMPILER} -DCONFIG=${BUILD_TYPE} -DUSE_LIBCXX=ON -DFORMAT_FILES_WITH_CLANG_FORMAT_BEFORE_EACH_BUILD=OFF -DCLANG_TIDY_ENABLE=OFF
make -j"$(sysctl -n hw.physicalcpu)"

ctest -VV -j"$(sysctl -n hw.physicalcpu)"
