#!/bin/bash

docker run --rm -v $(pwd):/source -e BUILD_TYPE=${BUILD_TYPE} -e CXX=${COMPILER} test
