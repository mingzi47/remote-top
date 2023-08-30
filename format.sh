#!/bin/bash

clang-format -i `find lib server/include -type f -name "*.h"`
clang-format -i `find lib server/src server/main.cpp  -type f -name "*.cpp"`

