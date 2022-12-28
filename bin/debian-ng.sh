#!/bin/bash

set -e

export PACKAGE=mrw-ng
export PREFIX=$PWD/rootfs/${PACKAGE}
export COPYRIGHT=${PREFIX}/usr/share/doc/${PACKAGE}/copyright
export BUILD_NUMBER=${BUILD_NUMBER:=0}
export ARCH=`dpkg --print-architecture`

rm -rf ${PREFIX}
test -f Makefile && make clean distclean
qmake -r
make -j `nproc`
make install

mkdir -p ${PREFIX}/usr/share/doc/${PACKAGE}
mkdir ${PREFIX}/DEBIAN
cp -a DEBIAN/control-ng  ${PREFIX}/DEBIAN/control

echo "Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/" > $COPYRIGHT
echo "Upstream-Name: $PACKAGE" >> ${COPYRIGHT}
echo "Source: https://github.com/stmork/mrw-track-control-ng" >> ${COPYRIGHT}
echo "" >> ${COPYRIGHT}
echo "Files: *" >> ${COPYRIGHT}
echo "Copyright: 2007-`date +'%Y'` Steffen A. Mork <linux-dev@morknet.de>" >> ${COPYRIGHT}
echo "License: MIT" >> ${COPYRIGHT}
sed -e 's/^$/\./g' -e 's/^/ /g' LICENSE.md >> ${COPYRIGHT}

sed -i\
   -e "s/%ARCH%/${ARCH}/g"\
   -e "s/%VERSION%/${VERSION}/g"\
   -e "s/%BUILD%/${BUILD_NUMBER}/g"\
   -e "s/%PACKAGE%/${PACKAGE}/g"\
   ${PREFIX}/DEBIAN/control
export VERSION=`grep Version ${PREFIX}/DEBIAN/control | cut -d" " -f2`

echo "Packaging..."
chmod -R o+rX ${PREFIX}
fakeroot dpkg -b ${PREFIX} ${PACKAGE}_${VERSION}_${ARCH}.deb
