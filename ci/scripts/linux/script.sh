#!/bin/bash

docker run --rm -v $(pwd):/source -e BUILD_TYPE=${BUILD_TYPE} -e CXX=${COMPILER} -e RC_PARAMS="${RC_PARAMS}" test
