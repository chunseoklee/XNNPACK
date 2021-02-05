#!/usr/bin/env bash
#
# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# Copyright 2019 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

set -e

mkdir -p build/android/armeabi-v7a


CMAKE_ARGS=()

# CMake-level configuration
CMAKE_ARGS+=("-DCMAKE_TOOLCHAIN_FILE=/home/twoflower/dev/ONE/infra/nnfw/cmake/buildtool/cross/toolchain_armv7l-linux.cmake")
CMAKE_ARGS+=("-DCMAKE_BUILD_TYPE=Release")
CMAKE_ARGS+=("-DCMAKE_POSITION_INDEPENDENT_CODE=ON")

# If Ninja is installed, prefer it to Make
if [ -x "$(command -v ninja)" ]
then
  CMAKE_ARGS+=("-GNinja")
fi

CMAKE_ARGS+=("-DXNNPACK_LIBRARY_TYPE=static")

CMAKE_ARGS+=("-DXNNPACK_BUILD_BENCHMARKS=ON")
CMAKE_ARGS+=("-DXNNPACK_BUILD_TESTS=ON")

# Cross-compilation options for Google Benchmark
CMAKE_ARGS+=("-DHAVE_POSIX_REGEX=0")
CMAKE_ARGS+=("-DHAVE_STEADY_CLOCK=0")
CMAKE_ARGS+=("-DHAVE_STD_REGEX=0")


# Use-specified CMake arguments go last to allow overridding defaults
CMAKE_ARGS+=($@)

cd build/android/armeabi-v7a && cmake ../../.. \
    "${CMAKE_ARGS[@]}"

# Cross-platform parallel build
if [ "$(uname)" == "Darwin" ]
then
  cmake --build . -- "-j$(sysctl -n hw.ncpu)"
else
  cmake --build . -- "-j$(nproc)"
fi
