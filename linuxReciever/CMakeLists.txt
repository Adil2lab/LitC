cmake_minimum_required(VERSION 3.18)
project(controller_receiver CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(receiver src/main.cpp src/controller-protocol.h)

# Set output directory
set_target_properties(receiver PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
