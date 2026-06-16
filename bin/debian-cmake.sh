#!/bin/bash

set -e

export PACKAGE=mrw-ng
export PREFIX=$PWD/rootfs/${PACKAGE}
export ARCH=${ARCH:-`dpkg --print-architecture`}
export BUILD=build-${ARCH}

echo "Arch:   $ARCH"

rm -rf ${PREFIX}
cmake -B ${BUILD} -DCMAKE_INSTALL_PREFIX=${PREFIX}
make -C ${BUILD} -j `nproc`
make -C ${BUILD} install

. $PWD/bin/debian.sh
