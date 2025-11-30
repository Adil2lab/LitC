cmake_minimum_required(VERSION 3.18.1)
project("controller")

add_library(controller SHARED controller.cpp)

find_library(log-lib log)

target_link_libraries(controller ${log-lib})