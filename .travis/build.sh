#!/bin/bash

set -x

mkdir build
mkdir build/TIMLE

cmake -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION -DTARGET_CPU=$TARGET_CPU -DCMAKE_INSTALL_PREFIX=./build/TIMLE -DCMAKE_PREFIX_PATH=./SFML/install . || exit 1
make || exit 1
make build || exit 1

#cmake .. -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION -DTARGET_CPU=$TARGET_CPU || exit 1
#cmake --build . || exit 1
