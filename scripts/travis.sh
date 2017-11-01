#!/bin/bash

set -e

# Go to source dir
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
cd ..

# Create a build folder
rm -rf travis-build
mkdir travis-build
cd travis-build
cmake -Dsanitize=On ..
make -j2
ctest -j2
