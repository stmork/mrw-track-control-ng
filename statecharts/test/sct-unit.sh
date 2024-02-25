#!/bin/bash

set -e

cd `dirname $0`

./MRW-Test-Statecharts --gtest_output=xml:gtest-mrw.xml

echo Completed.
