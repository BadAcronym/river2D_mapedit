#!/bin/bash

./clean
./run debug --compile-only
valgrind --tool=callgrind ./bin/debug/mapedit
kcachegrind ./callgrind.*
