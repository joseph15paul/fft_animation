#!/bin/bash
if [ "$#" -lt 2 ]; then
  ./build/animatedFFT --help
  exit 1
fi

FILE="$1"
VALUE="$2"

cmake -B build
cmake --build build
./build/animatedFFT "$@"

