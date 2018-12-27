#!/bin/bash

docker run --rm -v $(pwd):/source -v $(pwd)/build:/build test /package.sh
