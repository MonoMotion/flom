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

set -euo pipefail

SOURCE_DIR=/source
BUILD_DIR=/build

mkdir -p $BUILD_DIR && cd $BUILD_DIR
cmake -S $SOURCE_DIR -B $BUILD_DIR -DCMAKE_CXX_COMPILER=${CXX} -DCONFIG=${BUILD_TYPE} -DFORMAT_FILES_WITH_CLANG_FORMAT_BEFORE_EACH_BUILD=OFF -DCLANG_TIDY_ENABLE=OFF -DENABLE_TEST=${ENABLE_TEST:=ON}
make -j"$(nproc)"

if [ "$ENABLE_TEST" == "ON" ]; then
  ctest -VV -j"$(nproc)"
fi
