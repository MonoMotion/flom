#!/bin/bash

set -euo pipefail

mkdir /build && cd $_
cmake /source -DCMAKE_CXX_COMPILER=${CXX} -DCONFIG=${BUILD_TYPE} -DFORMAT_FILES_WITH_CLANG_FORMAT_BEFORE_EACH_BUILD=OFF -DCLANG_TIDY_ENABLE=OFF
make -j"$(nproc)"
