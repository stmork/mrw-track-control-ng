#!/bin/bash

for OS in debian ubuntu
do
   docker pull "${OS}:latest"
   docker build --no-cache -t "mrw-${OS}" --build-arg OS="${OS}" docker
done
