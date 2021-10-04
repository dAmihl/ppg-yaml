cmake_minimum_required (VERSION 3.13...3.21)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

project(ppg-yaml VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB PPG_YAML_SRC
    "src/*.h"
    "src/*.cpp"
)

file(GLOB PPG_YAML_EXAMPLE_SRC
    "example/*.h"
    "example/*.cpp"
)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(vendor/ppg-core)
add_subdirectory(vendor/yaml-cpp)

add_library(ppg-yaml STATIC ${PPG_YAML_SRC})

target_link_libraries(ppg-yaml PUBLIC ppg-core)
target_link_libraries(ppg-yaml PUBLIC yaml-cpp)

target_include_directories(ppg-yaml PUBLIC "vendor/ppg-core/src")

add_executable(ppg-yaml-example ${PPG_YAML_EXAMPLE_SRC})

set_target_properties(
    ppg-yaml-example PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/example")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ppg-yaml-example)

target_link_libraries(ppg-yaml-example PUBLIC ppg-yaml)

target_include_directories(ppg-yaml-example PUBLIC "src/")