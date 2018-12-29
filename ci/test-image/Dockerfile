#
# Copyright 2018 coord.e
#
# This file is part of Flom.
#
# Flom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Flom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Flom.  If not, see <http://www.gnu.org/licenses/>.
#

FROM debian:buster-slim

ARG BOOST_VERSION=1.69.0
ARG PROTOBUF_VERSION=3.6.1
ARG CMAKE_VERSION=3.13.2

ENV RC_PARAMS="verbose_progress=1 verbose_shrinking=1"
ENV CXX="clang++"
ENV CC="clang"
ENV BUILD_TYPE="Release"

RUN DEBIAN_FRONTEND=noninteractive \
    && apt-get update -y \
    && apt-get install -y --no-install-recommends build-essential clang libc++-dev libc++abi-dev wget bash git ruby ruby-dev rpm bsdtar \
    && gem install --no-document fpm

RUN mkdir boost && cd $_ \
    && BOOST_VERSION_U=$(echo $BOOST_VERSION | sed 's/\./_/g') \
    && BOOST_URL="https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_U}.tar.gz" \
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
    && ./configure CXXFLAGS=-fPIC \
    && make -j"$(nproc)" \
    && make check > /dev/null \
    && make install \
    && ldconfig \
    && cd ..

COPY build.sh /
COPY package.sh /

VOLUME ["/source", "/build"]
WORKDIR /source

CMD ["/build.sh"]
