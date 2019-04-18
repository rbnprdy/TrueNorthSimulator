cmake_minimum_required(VERSION 2.8.9)

project(TrueNorthSimulator)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(simulator ${SOURCES})