#!/bin/bash
if [ "$#" -lt 1 ]; then
  ./build/animatedFFT --help
  exit 1
fi

cmake -B build
cmake --build build
./build/animatedFFT "$@"

