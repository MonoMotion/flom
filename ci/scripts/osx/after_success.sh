#!/bin/bash

cd build

cpack -D CPACK_OUTPUT_FILE_PREFIX=dist
