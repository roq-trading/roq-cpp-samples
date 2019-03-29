#!/usr/bin/env bash

set -e

git submodule update --init --recursive
git submodule foreach 'git pull origin master && git checkout master'
