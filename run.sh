#!/bin/bash
set -e

cd build
make -j3
cd ..
./build/main
