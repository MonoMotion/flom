#!/bin/bash

brew upgrade cmake || brew install cmake
brew upgrade boost || brew install boost
brew upgrade protobuf || brew install protobuf
brew upgrade llvm || brew install llvm --with-clang
export PATH="/usr/local/opt/llvm/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib"
