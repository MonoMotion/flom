FROM debian:buster-slim

ARG BOOST_VERSION=1.69.0
ARG PROTOBUF_VERSION=3.6.1
ARG CMAKE_VERSION=3.13.2

ENV RC_PARAMS="verbose_progress=1 verbose_shrinking=1"
ENV CXX="clang++-7"
ENV CC="clang-7"
ENV BUILD_TYPE="Release"

RUN mkdir boost && cd $_ \
    && BOOST_URL="https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION//./_}.tar.gz" \
    && wget --no-check-certificate --quiet -O - "${BOOST_URL}" | tar --strip-components=1 -xz \
    && cp -r boost /usr/include \
    && cd ..

RUN mkdir cmake && cd $_ \
    && CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz" \
    && wget --no-check-certificate --quiet -O - "${CMAKE_URL}" | tar --strip-components=1 -xz \
    && cp bin/* /usr/bin/ \
    && cp -r share/* /usr/share/ \
    && cd ..

RUN mkdir protobuf && cd $_ \
    && PROTOBUF_URL="https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/protobuf-all-${PROTOBUF_VERSION}.tar.gz" \
    && wget --no-check-certificate --quiet -O - "${PROTOBUF_URL}" | tar --strip-components=1 -xz \
    && CXXFLAGS='-stdlib=libc++' CFLAGS='-fPIC' LDFLAGS='-stdlib=libc++ -lc++ -lc++abi -fPIC' ./configure \
    && make -j"$(nproc)" \
    && make check > /dev/null \
    && make install \
    && ldconfig \
    && cd ..

VOLUME ["/source"]
WORKDIR /source


CMD ["bash", "-c", "cmake . -DCMAKE_CXX_COMPILER=${CXX} -DCONFIG=${BUILD_TYPE} -DUSE_LIBCXX=ON -DFORMAT_FILES_WITH_CLANG_FORMAT_BEFORE_EACH_BUILD=OFF -DCLANG_TIDY_ENABLE=OFF && make -j$(nproc)"]
