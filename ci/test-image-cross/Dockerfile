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

ARG BASE_IMAGE=raspberry-pi
ARG DEBIAN_VERSION=stretch
FROM balenalib/${BASE_IMAGE}:${DEBIAN_VERSION}-build

ENV CXX="g++"
ENV CC="gcc"
ENV BUILD_TYPE="Release"

COPY image-scripts/install_common.sh /
COPY image-scripts/install_specific.sh /

RUN ["cross-build-start"]

RUN /install_common.sh
RUN /install_specific.sh

RUN ["cross-build-end"]

COPY image-scripts/build.sh /
COPY image-scripts/package.sh /

WORKDIR /source

ENTRYPOINT ["/usr/bin/resin-xbuild"]

CMD ["-c", "bash /build.sh"]
