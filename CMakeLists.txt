cmake_minimum_required (VERSION 2.6)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
project (ppg-yaml)
SET (PROJECT_SOURCE_DIR "src")
SET (PROJECT_HEADERS_DIR "src")
SET (INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/include")
SET (LIB_DIRECTORY "${CMAKE_SOURCE_DIR}")

# CMake instructions to make the static lib

ADD_LIBRARY(ppg-yaml STATIC
            ${PROJECT_SOURCE_DIR}/Yaml2Puzzle.cpp
			)

find_package(Yaml-cpp)
target_link_libraries(${PROJECT_NAME} ${Yaml-cpp_LIBS})

find_package(ppg-core)
target_link_libraries(${PROJECT_NAME} ${ppg-core_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORY} ${LIB_DIRECTORY}/ppg-core/include)

set_target_properties(ppg-yaml PROPERTIES 
		PUBLIC_HEADER 	"${PROJECT_HEADERS_DIR}/Yaml2Puzzle.h;")
		
INSTALL(TARGETS ppg-yaml 
        LIBRARY DESTINATION lib/
        PUBLIC_HEADER DESTINATION includes/
)