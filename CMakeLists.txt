project(InterruptibleSleeper)
cmake_minimum_required(VERSION 3.0)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(test)
