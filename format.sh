#!/bin/bash

clang-format -i `find monitor/include/ server/include log -type f -name "*.h"`
clang-format -i `find monitor/ server/   -type f -name "*.cpp"`

