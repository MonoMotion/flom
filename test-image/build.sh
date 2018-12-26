#!/bin/bash

set -euo pipefail

BUILD_DIR=$1

mkdir -p $BUILD_DIR && cd $_
cmake /source -DCMAKE_CXX_COMPILER=${CXX} -DCONFIG=${BUILD_TYPE} -DFORMAT_FILES_WITH_CLANG_FORMAT_BEFORE_EACH_BUILD=OFF -DCLANG_TIDY_ENABLE=OFF
make -j"$(nproc)"
