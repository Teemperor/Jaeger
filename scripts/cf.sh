#!/bin/bash

set -e

# Go to source dir
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"
cd ..

find . -name "*.cpp" | xargs clang-format -i
find . -name "*.h" | xargs clang-format -i
