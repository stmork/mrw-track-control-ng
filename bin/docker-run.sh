#!/bin/bash

OS=${1:-ubuntu}

docker run --network=host -it "mrw-${OS}" bash
