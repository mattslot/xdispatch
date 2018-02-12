#!/bin/sh
mkdir build_mac
cd build_mac
cmake ..
cd ..
# cmake --build build_mac --config Debug
cmake --build build_mac --config Release