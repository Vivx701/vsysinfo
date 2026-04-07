#!/bin/bash

set -e

BUILD_DIR="build"

echo "=== Creating build directory ==="
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "=== Configuring with CMake ==="
cmake .. -DCMAKE_BUILD_TYPE=Release

echo "=== Building ==="
cmake --build . -j$(nproc)

echo "=== Running tests ==="
ctest --output-on-failure

echo "=== Build complete ==="