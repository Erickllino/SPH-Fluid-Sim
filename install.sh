#!/bin/bash
set -e

rm -rf build
mkdir build
cd build
cmake ..
cd ..
chmod +x run.sh
./run.sh

