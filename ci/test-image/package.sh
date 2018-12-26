#!/bin/bash

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
  local ext=${2:-$type}

  cd $INPUT
  fpm -s dir -t $type -n flom -v $VERSION -m "coord.e <me@coord-e.com>" --url "https://github.com/DeepL2/flom" --description "Motion data exchange format" --prefix $PREFIX -p ../${OUTPUT}.$ext *
  cd ..
}

build_package deb
build_package rpm
build_package pacman pkg.tar.xz
