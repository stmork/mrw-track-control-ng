#!/bin/bash

set -e

umask 022
GTEST_SRC=/usr/src/gtest/
test -d $GTEST_SRC
cd $GTEST_SRC

cmake CMakeLists.txt
make
cd /usr/local/lib/
ln -s $GTEST_SRC/*.a .

