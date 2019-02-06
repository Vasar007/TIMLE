#!/bin/bash

set -x

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 90

    CMAKE_VERSION=3.8.2
    CMAKE_VERSION_DIR=v3.8

    CMAKE_OS=Linux-x86_64
    CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
    CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
    CMAKE_DIR=$(pwd)/cmake-$CMAKE_VERSION

    wget --quiet $CMAKE_URL
    mkdir -p $CMAKE_DIR
    tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR
    export PATH=$CMAKE_DIR/bin:$PATH
else
    brew update
fi

git clone https://github.com/SFML/SFML.git
cd SFML
mkdir install
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DSFML_DEPENDENCIES_INSTALL_PREFIX=../install -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE
make
make install
cd ../../
