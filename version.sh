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

VERSION_TXT="VERSION.txt"

cd "$(dirname "$0")"

function get_meta() {
  local branch=$(git name-rev HEAD | sed 's/.* \([^~]*\).*/\1/')

  if [ "$branch" == "master" ]; then
    exit
  fi

  echo -n "+"

  if [ "${TRAVIS_PULL_REQUEST:-false}" != false ]; then
    echo "pr_$TRAVIS_PULL_REQUEST"
  else
    echo "${branch//[^[:alnum:]-]/-}"
  fi
}

function main() {
  local base_version=$(cat $VERSION_TXT)
  local meta_version=$(get_meta)

  echo "${base_version}${meta_version}"
}

main


