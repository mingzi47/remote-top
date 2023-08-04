#!/bin/bash

clang-format -i `find monitor/include/ server/include -type f -name *.h`
clang-format -i `find monitor/ server/  -type f -name *.cpp`

