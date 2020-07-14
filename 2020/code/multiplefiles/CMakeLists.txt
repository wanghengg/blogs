cmake_minimum_required(VERSION 3.10)

project(SafeIntAdd)

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -g -Wall)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(lib/ definition/)

aux_source_directory(./ SOURCE_DIR)

add_executable(SafeIntAdd ${SOURCE_DIR})