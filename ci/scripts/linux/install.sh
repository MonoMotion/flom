#!/bin/bash

docker build ci/test-image/ -t test

apt-get update -y
apt-get install -y ruby
gem install --no-document fpm
