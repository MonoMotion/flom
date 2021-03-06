#!/bin/bash
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

if [ -v DEBIAN_VERSION ]; then
  DEBIAN_VERSION_ARG="--build-arg DEBIAN_VERSION=${DEBIAN_VERSION}"
else
  DEBIAN_VERSION_ARG=
fi

travis_wait 30 docker build ci/ -f ci/test-image-cross/Dockerfile -t builder --build-arg BASE_IMAGE=${BASE_IMAGE} ${DEBIAN_VERSION_ARG}
