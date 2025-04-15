#!/bin/bash

set -e

export PACKAGE=mrw-ng
export PREFIX=$PWD/rootfs/${PACKAGE}
export QMAKE=${QMAKE:-qmake6}

echo "Arch:   $ARCH"
echo "qmake:  $QMAKE"

rm -rf ${PREFIX}
test -f Makefile && make clean distclean
${QMAKE} CONFIG+=qt-ss -r
make -j `nproc`
make install

. $PWD/bin/debian.sh
