cmake_minimum_required (VERSION 3.13...3.21)

project(ppg-yaml VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB PPG_YAML_SRC
    "src/*.h"
    "src/*.cpp"
)

add_library(ppg-yaml ${PPG_YAML_SRC})