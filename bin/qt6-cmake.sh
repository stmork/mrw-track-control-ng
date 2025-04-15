#!/bin/bash

set -e

export PREFIX=$PWD/rootfs/${PACKAGE}
export DEB_CONTROL=control-ng-qt6

BUILD_DIR=build
ARCH_TYPE=`dpkg-architecture -qDEB_HOST_GNU_TYPE`

rm -rf ${BUILD_DIR} ${PREFIX}
cmake \
   -DQT_HOST_PATH=/ \
   -DQT_HOST_PATH_CMAKE_DIR=/usr/lib/${ARCH_TYPE}/cmake \
   -DCMAKE_BUILD_TYPE=RELEASE \
   $@ \
   -B ${BUILD_DIR}
cmake --build   ${BUILD_DIR} --config Release --parallel `nproc`
cmake --install ${BUILD_DIR} --config Release

. $PWD/bin/debian.sh
