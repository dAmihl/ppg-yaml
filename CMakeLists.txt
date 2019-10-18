cmake_minimum_required (VERSION 2.6)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
project (ppg-yaml)
SET (PROJECT_SOURCE_DIR "src")
SET (PROJECT_HEADERS_DIR "src")
SET (INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/includes")
SET (LIB_DIRECTORY "${CMAKE_SOURCE_DIR}")

# CMake instructions to make the static lib

ADD_LIBRARY(ppg-yaml STATIC
	${PROJECT_SOURCE_DIR}/Yaml2Puzzle.cpp
	)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORY})

set_target_properties(ppg-yaml PROPERTIES 
		PUBLIC_HEADER 	"${PROJECT_HEADERS_DIR}/Yaml2Puzzle.h;")
		
INSTALL(TARGETS ppg-yaml 
        LIBRARY DESTINATION lib/
        PUBLIC_HEADER DESTINATION includes/
)
