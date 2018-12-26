#!/bin/bash

set -euo pipefail

BUILD_DIR=/build

cd $BUILD_DIR
cpack -D CPACK_OUTPUT_FILE_PREFIX=dist

