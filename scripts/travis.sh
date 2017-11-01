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

echo "Running clang-format"
cd "$DIR"
./cf.sh
git diff
git diff-index --quiet HEAD -- || echo "Echo clang-format failed!"
git diff-index --quiet HEAD -- || exit 1
