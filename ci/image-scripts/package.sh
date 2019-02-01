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
DIST_DIR=./dist

cd $BUILD_DIR
cpack -D CPACK_OUTPUT_FILE_PREFIX=$DIST_DIR

PREFIX=/usr
VERSION=$(${SOURCE_DIR}/version.sh)
ARCHIVE=$(find $DIST_DIR -name '*.tar.gz')
OUTPUT=${ARCHIVE%.tar.gz}
INPUT=./artifact

# TODO: This is not cool
mkdir -p $INPUT && cd $INPUT
tar --strip-components=1 -xf ../$ARCHIVE
cd ..

function build_package() {
  local type=$1
  local ext=$2
  local version=$3
  shift 3

  cd $INPUT
  fpm -s dir -t $type -n flom -v $version -m "coord.e <me@coord-e.com>" --url "https://github.com/MonoMotion/flom" --description "Motion data exchange format" "$@" --prefix $PREFIX -p ../${OUTPUT}.$ext *
  cd ..
}

build_package deb deb ${VERSION} -d "libprotobuf10 (>= 3.0.0) | libprotobuf17 (>= 3.0.0)"
build_package pacman pkg.tar.xz ${VERSION//[^[:alnum:].]/} -d "protobuf >= 3.0.0"
