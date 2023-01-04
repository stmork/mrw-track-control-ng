#!/bin/bash

set -e

cd `dirname $0`
OperatingMode/MRW-Test-OpMode --gtest_output=xml:gtest-opmode.xml
Route/MRW-Test-Route --gtest_output=xml:gtest-route.xml
Section/MRW-Test-Section --gtest_output=xml:gtest-section.xml
Signal/MRW-Test-Signal --gtest_output=xml:gtest-signal.xml
SignalController/MRW-Test-SignalController --gtest_output=xml:gtest-signalcontroller.xml
Switch/MRW-Test-Switch --gtest_output=xml:gtest-switch.xml
Tracker/MRW-Test-Tracker --gtest_output=xml:gtest-tracker.xml
Update/MRW-Test-Update --gtest_output=xml:gtest-updater.xml

echo Completed.
