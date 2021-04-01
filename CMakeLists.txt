project(HW-2 C CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "--coverage -Wall -Werror -Wpedantic -g -pthread")

enable_testing()

add_subdirectory(project)
