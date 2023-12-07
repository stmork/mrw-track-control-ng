#!/bin/bash

test -f Makefile && make clean
for DIR in api-doc cccc lcov-out rootfs
do
   rm -rf ${DIR}
done
