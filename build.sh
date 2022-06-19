#!/bin/sh

PRESET="mac-native"        # could be any of the macOS presets in CMakePresets.json
BUILD_CONFIGURATION="Release" # could be "Debug" as well

cmake -S . --preset "${PRESET}"
cmake --build --preset "${PRESET}" --config "${BUILD_CONFIGURATION}" --parallel
cmake --install "build/${PRESET}" --config ${BUILD_CONFIGURATION}
