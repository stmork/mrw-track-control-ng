#!/bin/bash

OS=${1:-ubuntu}

docker run -it "mrw-${OS}" bash
