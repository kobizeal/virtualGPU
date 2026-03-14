cmake_minimum_required(VERSION 3.20)
project(virtual_gpu)

set(CMAKE_CXX_STANDARD 17)

add_executable(virtual_gpu
        main.cpp
        hardware.cpp
        kernel.cpp
)