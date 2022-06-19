@echo off

:: Could be any of the Windows presets in CMakePresets.json
set PRESET=windows-native

:: Build configuration - could be "Debug" as well
set BUILD_CONFIGURATION=Release
set BUILD_DIR=build\%BUILD_CONFIGURATION%

:: Create the configuration file using cmake
cmake -S . --preset %PRESET%

:: Now build the project
cmake --build --preset %PRESET% --config %BUILD_CONFIGURATION% --parallel

:: Install the extension
cmake --install build\%PRESET% --config %BUILD_CONFIGURATION%
