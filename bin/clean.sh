#!/bin/bash

test -f Makefile && make clean
rm -rf *.deb
for DIR in api-doc cccc lcov-out rootfs build
do
   rm -rf ${DIR}
done
