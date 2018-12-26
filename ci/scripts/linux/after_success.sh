#!/bin/bash

docker run --rm -v $(pwd):/source -v $(pwd)/build:/build test /package.sh

PREFIX=/usr
VERSION=$(${TRAVIS_BUILD_DIR}/version.sh)
INPUT=$(find dist -name '*.tar.gz')
OUTPUT=$(basename ${INPUT%.tar.gz})

fpm -s tar -t deb -n flom -v $VERSION --prefix $PREFIX -p ${OUTPUT}.deb $INPUT
