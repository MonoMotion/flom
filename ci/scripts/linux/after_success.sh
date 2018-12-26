#!/bin/bash

docker run --rm -v $(pwd):/source -v $(pwd)/build:/build test /package.sh

PREFIX=/usr
VERSION=$(${TRAVIS_BUILD_DIR}/version.sh)
INPUT=$(find ${TRAVIS_BUILD_DIR}/build/dist -name '*.tar.gz')
OUTPUT=${INPUT%.tar.gz}

function build_package() {
  local type=$1
  local ext=${2:-$type}
  fpm -s tar -t $type -n flom -v $VERSION --prefix $PREFIX -p ${OUTPUT}.$ext $INPUT
}

build_package deb
build_package rpm
build_package pacman pkg.tar.xz
