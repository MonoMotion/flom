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

set -euo pipefail

DEBIAN_FRONTEND=noninteractive

apt-get update -y
apt-get install -y --no-install-recommends build-essential wget clang libc++-dev libc++abi-dev bash git ruby ruby-dev rpm bsdtar libboost-dev cmake libprotobuf-dev protobuf-compiler libeigen3-dev
gem install --no-document rake fpm
