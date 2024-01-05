cmake_minimum_required(VERSION 2.8)
project(runner)
set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src/ims)
include_directories(${PROJECT_SOURCE_DIR}/src/runners)

add_executable(runner ${PROJECT_SOURCE_DIR}/src/runners/runner.cpp)
add_library(IMS STATIC ${PROJECT_SOURCE_DIR}/src/ims/ims.hpp)
set_target_properties(IMS PROPERTIES LINKER_LANGUAGE CXX)
add_library(machine STATIC ${PROJECT_SOURCE_DIR}/src/ims/machine.hpp)
set_target_properties(machine PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(runner IMS machine)
