#!/bin/bash

# Fail on any error.
set -e

# Move to root of project.
cd "$(cd -P "$(dirname "$0")"/.. && pwd)"

# Remove cmake files that aren't required.
find -name CMakeCache.txt -o \
     -name CMakeFiles -o \
     -name CMakeScripts -o \
     -name cmake_install.cmake | xargs rm -rf

# export CC=${CC:-"clang-13"}
# export CXX=${CXX:-"clang++-13"}

cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --clean-first
cmake --build build --config Release --target test -- CTEST_OUTPUT_ON_FAILURE=1
