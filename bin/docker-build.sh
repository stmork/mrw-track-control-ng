#!/bin/bash

for OS in debian ubuntu
do
   docker build -t "mrw-${OS}" --build-arg OS="${OS}" docker
done
