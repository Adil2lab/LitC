cmake_minimum_required(VERSION 3.22)

project(controller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(controller SHARED controller-protocol.h controller.cpp)

find_library(log-lib log)
target_link_libraries(controller ${log-lib})