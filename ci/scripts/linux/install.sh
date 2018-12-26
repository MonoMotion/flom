#!/bin/bash

docker build ci/test-image/ -t test

apt-get update -y
apt-get install -y ruby git
gem install --no-document fpm
