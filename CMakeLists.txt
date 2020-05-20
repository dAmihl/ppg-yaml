cmake_minimum_required (VERSION 2.6)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
project (ppg-yaml)
SET (SOURCES_DIR "src")
SET (PROJECT_HEADERS_DIR "src")
SET (INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/includes")
SET (LIB_DIRECTORY "${PROJECT_SOURCE_DIR}")

# CMake instructions to make the static lib

SET (YAML_CPP_SUBDIR "${PROJECT_SOURCE_DIR}/includes/thirdparty/yaml-cpp")
SET (PPG_CORE_SUBDIR "${PROJECT_SOURCE_DIR}/includes/ppg-core")

add_subdirectory(${YAML_CPP_SUBDIR})
add_subdirectory(${PPG_CORE_SUBDIR})

link_directories( "${PROJECT_SOURCE_DIR}/includes/yaml-cpp/build")
link_directories("${PROJECT_SOURCE_DIR}/includes/ppg-core/build")

ADD_LIBRARY(ppg-yaml STATIC
	${SOURCES_DIR}/Yaml2Puzzle.cpp
	)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORY} ${INCLUDE_DIRECTORY}/ppg-core/include/ ${INCLUDE_DIRECTORY}/thirdparty/yaml-cpp/include/)

#find_package(yaml-cpp PATHS ${INCLUDE_DIRECTORY}/thirdparty/yaml-cpp/build)

target_link_libraries(ppg-yaml PUBLIC ppg-core)
target_link_libraries(ppg-yaml PUBLIC yaml-cpp)

set_target_properties(ppg-yaml PROPERTIES 
		PUBLIC_HEADER 	"${PROJECT_HEADERS_DIR}/Yaml2Puzzle.h;")
		
INSTALL(TARGETS ppg-yaml 
        LIBRARY DESTINATION lib/
        PUBLIC_HEADER DESTINATION includes/ppg-yaml
)
